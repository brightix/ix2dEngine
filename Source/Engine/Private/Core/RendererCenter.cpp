#include "Classes/Core/RendererCenter.hpp"

#include <set>
#include <thread>
#include <SDL3/SDL_render.h>
#include "Types/RenderData.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "Utilities/FuncLib/Deleter.hpp"
#include "Classes/Widget/Widget.hpp"
#include <vector>

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Widget/PanelWidget/PanelWidget.hpp"
std::condition_variable RendererCenter::render_cv;
std::atomic<bool> RendererCenter::is_stop = false;
SDL_Renderer* RendererCenter::renderer = nullptr;
bool RendererCenter::is_multithreading = false;
RendererCenter::RendererCenter() : window(nullptr)
{
}

void RendererCenter::Init()
{
	if (is_multithreading)
	{
		StartRenderThread();
	}
	else
	{
		InitSDL();
		event_system.AddEvent(Event("OnRenderSceneDataReady",[&](TEventParams e) {
			auto clips = std::move(*e->Get<std::vector<RenderData>>("render_data"));
			RenderScene(clips);
		}));
		event_system.AddEvent(Event("OnRenderWidgetDataReady",[&](TEventParams e) {
			std::vector<GCWeakPtr<Widget>> clips = std::move(*e->Get<std::vector<GCWeakPtr<Widget>>>("widget_data"));
			auto viewport = (*e->Get<GCWeakPtr<GameWorld>>("widget_data"))->viewport;
			RenderWidget();
		}));
		event_system.AddEvent(Event("OnRenderPresent",[&](TEventParams e) {
			SDL_RenderPresent(renderer);
		}));
		event_system.AddEvent(Event("OnRenderClear",[&](TEventParams e) {
			SDL_SetRenderDrawColor(renderer, 100, 100, 100,0);
			SDL_RenderClear(renderer);
			SDL_RenderClear(renderer);
		}));
		DefaultTexture = CreateOutLineTexture(FRect(0,0,10,10));
	}
}

void RendererCenter::DeInit()
{
	//多线程下会自己调用反初始化
	if (is_multithreading)
	{
		Quit();
	}
	else
	{
		//主线程需要主动调用
		DeInitSDL();
	}
	if (render_thread.joinable())
	{
		render_thread.join();
	}
}

void RendererCenter::StartRenderThread()
{
	if (is_multithreading)
	{
		//回应渲染包准备完成
		event_system.AddEvent(Event("HandleRenderDataReady",[&](TEventParams e) {
			//最新帧为true
			int current = render_idx.load() % 2;
			render_fence[current].store(true);
			auto ep = *e->Get<std::vector<RenderData>>("render_data");
			render_list[current] = ep;

			if (!render_fence[(render_idx+1) % 2].load())
			{
				return;
			}
			auto clips = std::move(render_list[current]);

			RenderScene(clips);

			//标记当前渲染完的帧可以被覆写
			render_fence[current].store(false);

			//最新帧位置++
			render_idx.store(render_idx.load()+1);
		}));



		render_thread = std::thread([this]()
		{
			InitSDL();
			while (!is_stop.load())
			{
				{
					std::unique_lock lock(render_mtx);
					//等待第二帧数据准备完成
					render_cv.wait(lock, [this] {
						bool flag = is_stop.load();
						{
							std::lock_guard<std::mutex> lock(RendererTask_MTX);
							flag |= !priority_task.empty();
						}
						return render_fence[(render_idx+1) % 2].load() || flag;
					});
				}
				if (is_stop.load())
				{
					break;  // ✅ 一旦收到退出信号，立刻跳出，别再用renderer
				}
				{
					RenderTask task;
					{
						std::lock_guard lock(RendererTask_MTX);
						if (priority_task.empty())
							break;
						task = priority_task.top();
						priority_task.pop();
					}
					auto e = task();
					if (task.callback)
					{
						{
							std::lock_guard lock(left_callback_mtx);
							left_callback.emplace(std::move(task.callback),e);
						}
					}
				}
			}
			DeInitSDL();
		});
	}
	else
	{
		LogWithLevel("未开启多线程渲染也使用了GPU线程",FatalError);
	}
}

void RendererCenter::PushRenderData(const std::vector<RenderData>& render_data)
{
	int current = render_idx % 2;
	render_list[current] = render_data;
	render_fence[current].store(true);
	render_cv.notify_one();
}

void RendererCenter::AddRendererTask(RenderTask task)
{
	// 如开启多线程就使用消息队列  否则直接运行
	if (is_multithreading)
	{
		{
			std::lock_guard lock(RendererTask_MTX);
			if (is_stop.load())
			{
				return;
			}
			priority_task.emplace(task);
		}
		render_cv.notify_all();
	}
	else
	{

		if (task.callback)
		{
			task.callback(task());
		}
		else
		{
			task();
		}
	}
}


SDL_Renderer* RendererCenter::GetRenderer() const
{
	return renderer;
}

void RendererCenter::Quit()
{
	is_stop.store(true);
	render_cv.notify_all();
}

void RendererCenter::RenderScene(std::vector<RenderData>& clips)
{
	//数据预处理
	std::ranges::sort(clips.begin(),clips.end(),[](const RenderData& A,const RenderData& B){ return A.layer < B.layer;});
	// 清屏
	for (auto& clip : clips)
	{
		SDL_FRect* src = nullptr;
		if (clip.src_rect)
		{
			src = &*clip.src_rect;
		}
		if (clip.dst_rect)
		{
			const SDL_FRect* dst = &*clip.dst_rect;
			SDL_RenderTexture(renderer,clip.texture.get(),src,dst);
		}
	}
}

void RendererCenter::RenderWidget(GCWeakPtr<PanelWidget> viewport)
{
	//std::unordered_set<GCPtr<Widget>>& widgets = clips;
	const auto& [w,h] = GameEngine::Instance().GetViewportSize();
	viewport->WidgetRender(FRect(0,0,w,h));
}

std::shared_ptr<SDL_Texture> RendererCenter::CreateOutLineTexture(const FRect& rect)
{
	auto texture_T = TTexture(nullptr);
	SDL_Texture* t;
	t = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		rect.w,
		rect.h
	);
	texture_T.reset(t);
	SDL_SetRenderTarget(renderer, texture_T.get());
	// 清空背景
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // 透明背景
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // 青色边框
	int thickness = 3;
	for (int i = 0; i < thickness; ++i)
	{
		auto rect_bound = SDL_FRect(i,i, rect.w - i * 2, rect.h - i * 2);
		SDL_RenderRect(renderer, &rect_bound);
	}
	SDL_SetRenderTarget(renderer,nullptr);
	return texture_T;
}
SDL_Texture * RendererCenter::CreateFilledTexture(const FRect& rect)
{
	SDL_Texture* texture_T = SDL_CreateTexture(
	renderer,
	SDL_PIXELFORMAT_RGBA8888,
	SDL_TEXTUREACCESS_TARGET,
	rect.w,
	rect.h
);
	SDL_SetRenderTarget(renderer, texture_T);
	const auto rect_T = SDL_FRect(0, 0, rect.w, rect.h);
	SDL_RenderFillRect(renderer,&rect_T);
	SDL_SetRenderTarget(renderer, nullptr);
	return texture_T;
}

void RendererCenter::SetTextureFromSurface(Texture* t, std::shared_ptr<SDL_Surface> new_surface)
{
	if (is_multithreading)
	{
		RenderTask task;
		task.task = [new_surface]() mutable -> EventParams {
			EventParams e;
			e.Add("new_texture", TTexture(SDL_CreateTextureFromSurface(renderer, new_surface.get())));
			return e;
		};
		task.callback = [t](EventParams e) {
			t->SetStaticTexture(*e.Get<std::shared_ptr<SDL_Texture>>("new_texture"));
		};
		NewRendererTask(task);
	}
	else
	{
		auto new_texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, new_surface.get()),SDLTextureDeleter());
		t->SetNewTexture(new_texture);
	}
}

void RendererCenter::AsyncGetTextureFromSurface(GCWeakPtr<Texture> owner, std::shared_ptr<SDL_Surface> new_surface)
{
	RenderTask task;
	auto tex = TTexture(nullptr);
	//渲染线程修改临时纹理
	task.task = [new_surface]() mutable{
		EventParams e;
		e.Add("new_texture", TTexture(SDL_CreateTextureFromSurface(renderer, new_surface.get())));
		return e;
	};
	//主线程赋值
	task.callback = [owner](EventParams e) {
		if (auto obj = owner.Peek())
		{
			obj->SetStaticTexture(*e.Get<std::shared_ptr<SDL_Texture>>("new_texture"));
		}
	};
	NewRendererTask(task);
}

void RendererCenter::ReadLeftCallback()
{
	if (!is_multithreading)
	{
		return;
	}
	while (true)
	{
		std::pair<std::function<void(EventParams)>,EventParams> callback;

		{
			std::lock_guard lock(left_callback_mtx);
			if (left_callback.empty()) return;
			callback = std::move(left_callback.front());
			left_callback.pop();
		}
		callback.first(callback.second);
	}
}


void RendererCenter::InitSDL()
{

	window = SDL_CreateWindow(
		"Hello SDL3",        // 标题
		1200, 1000,            // 宽高
		SDL_WINDOW_RESIZABLE // 可拉伸
	);
	if (!window)
	{
		Log("SDL_CreateWindow Error:" + std::string(SDL_GetError()));
		SDL_Quit();
		return;
	}
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer)
	{
		Log("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

void RendererCenter::DeInitSDL() const
{
	SDL_GetError();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


//单线程专用
