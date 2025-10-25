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
SDL_Renderer* RendererCenter::renderer = nullptr;
RendererCenter::RendererCenter() : window(nullptr)
{
}

void RendererCenter::Init()
{
	InitSDL();
	NAME;
	event_system.AddEvent(Event("OnRenderSceneDataReady",[&](TEventParams e) {
		auto clips = std::move(*e->Get<std::vector<RenderData>>("render_data"));
		RenderScene(clips);
	}));
	event_system.AddEvent(Event("OnRenderWidgetDataReady",[&](TEventParams e) {
		//std::vector<GCWeakPtr<Widget>> clips = std::move(*e->Get<std::vector<GCWeakPtr<Widget>>>("widget_data"));
		auto viewport = (*e->Get<GCWeakPtr<GameWorld>>("widget_data"))->viewport;
		RenderWidget(viewport);
	}));
	event_system.AddEvent(Event("OnRenderPresent",[&](TEventParams e) {
		SDL_RenderPresent(renderer);
	}));
	event_system.AddEvent(Event("OnRenderClear",[&](TEventParams e) {
		SDL_SetRenderDrawColor(renderer, 100, 100, 100,0);
		SDL_RenderClear(renderer);
		SDL_RenderClear(renderer);
	}));
	DefaultTexture = TTexture(CreateOutLineTexture({10.f,10.f}));
}

void RendererCenter::DeInit()
{
	DeInitSDL();
}



SDL_Renderer* RendererCenter::GetRenderer() const
{
	return renderer;
}

void RendererCenter::RenderScene(std::vector<RenderData>& clips)
{
	//数据预处理
	std::ranges::sort(clips.begin(),clips.end(),[](const RenderData& A,const RenderData& B){ return A.layer < B.layer;});

	for (auto& clip : clips)
	{
		auto& [_, texture,trans,src_opt,dst_opt] = clip;
		auto point = SDL_FPoint(trans.rotation.Point->x,trans.rotation.Point->y);
		SDL_FRect* src = nullptr;
		SDL_FRect* dst = nullptr;
		if (src_opt)
		{
			src = &*src_opt;
		}
		if (dst_opt)
		{
			dst = &*dst_opt;
		}
		//SDL_RenderTexture(renderer,texture.get(),src,dst);
		SDL_RenderTextureRotated(renderer,texture.get(),src,dst,trans.rotation.Angle,&point,SDL_FLIP_NONE);
	}
}

void RendererCenter::RenderWidget(GCWeakPtr<PanelWidget> viewport)
{
	//std::unordered_set<GCPtr<Widget>>& widgets = clips;
	const auto& [w,h] = GameEngine::Instance().GetViewportSize();
	viewport->NativeWidgetRender(FRect(0,0,w,h));
}

SDL_Texture* RendererCenter::CreateOutLineTexture(const Vec2<float>& size, SDL_Color color)
{
	if (!renderer)
	{
		Log("renderer 非法");
	}
	SDL_Texture* t = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		size.x,
		size.y
	);
	SDL_SetRenderTarget(renderer, t);
	// 清空背景
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // 透明背景
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r,color.g,color.b,color.a); // 青色边框
	int thickness = 3;
	for (int i = 0; i < thickness; ++i)
	{
		auto rect_bound = SDL_FRect(i,i, size.x - i * 2, size.y - i * 2);
		SDL_RenderRect(renderer, &rect_bound);
	}
	SDL_SetRenderTarget(renderer,nullptr);
	return t;
}
SDL_Texture * RendererCenter::CreateFilledTexture(const Vec2<int> size)
{
	SDL_Texture* texture_T = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		size.x,
		size.y
	);
	SDL_SetRenderTarget(renderer, texture_T);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	const auto rect_T = SDL_FRect(0, 0, size.x, size.y);
	SDL_RenderFillRect(renderer,&rect_T);
	SDL_SetRenderTarget(renderer, nullptr);
	return texture_T;
}

void RendererCenter::SetTextureFromSurface(Texture* t, std::shared_ptr<SDL_Surface> new_surface)
{
	auto s = new_surface.get();
	auto new_texture = SDL_CreateTextureFromSurface(renderer, s);
	auto new_texture_s = std::shared_ptr<SDL_Texture>(new_texture,SDLTextureDeleter());
	t->SetNewTexture(new_texture_s);
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

std::shared_ptr<SDL_Texture> RendererCenter::GetDefaultTexture()
{
	return DefaultTexture;
}


//单线程专用

// void RendererCenter::StartRenderThread()
// {
// 	if (is_multithreading)
// 	{
// 		//回应渲染包准备完成
// 		event_system.AddEvent(Event("HandleRenderDataReady",[&](TEventParams e) {
// 			//最新帧为true
// 			const int current = render_idx.load() % 2;
// 			render_fence[current].store(true);
// 			auto ep = *e->Get<std::vector<RenderData>>("render_data");
// 			render_list[current] = ep;
//
// 			if (!render_fence[(render_idx+1) % 2].load())
// 			{
// 				return;
// 			}
// 			auto clips = std::move(render_list[current]);
//
// 			RenderScene(clips);
//
// 			//标记当前渲染完的帧可以被覆写
// 			render_fence[current].store(false);
//
// 			//最新帧位置++
// 			render_idx.store(render_idx.load()+1);
// 		}));
//
//
//
// 		render_thread = std::thread([this]()
// 		{
// 			InitSDL();
// 			while (!is_stop.load())
// 			{
// 				{
// 					std::unique_lock lock(render_mtx);
// 					//等待第二帧数据准备完成
// 					render_cv.wait(lock, [this] {
// 						bool flag = is_stop.load();
// 						{
// 							std::lock_guard<std::mutex> lock(RendererTask_MTX);
// 							flag |= !priority_task.empty();
// 						}
// 						return render_fence[(render_idx+1) % 2].load() || flag;
// 					});
// 				}
// 				if (is_stop.load())
// 				{
// 					break;  // ✅ 一旦收到退出信号，立刻跳出，别再用renderer
// 				}
// 				{
// 					RenderTask task;
// 					{
// 						std::lock_guard lock(RendererTask_MTX);
// 						if (priority_task.empty())
// 							break;
// 						task = priority_task.top();
// 						priority_task.pop();
// 					}
// 					auto e = task();
// 					if (task.callback)
// 					{
// 						{
// 							std::lock_guard lock(left_callback_mtx);
// 							left_callback.emplace(std::move(task.callback),e);
// 						}
// 					}
// 				}
// 			}
// 			DeInitSDL();
// 		});
// 	}
// 	else
// 	{
// 		LogWithLevel("未开启多线程渲染也使用了GPU线程",FatalError);
// 	}
// }
