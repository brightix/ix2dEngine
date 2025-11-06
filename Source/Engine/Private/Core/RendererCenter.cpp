#include "Classes/Core/RendererCenter.hpp"

#include <set>
#include <thread>
#include <SDL3/SDL_render.h>
#include <vector>
#include "Types/RenderData.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "Utilities/FuncLib/Deleter.hpp"
#include "Classes/Widget/Widget.hpp"
#include "../../Asset/Texture.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Widget/PanelWidget/PanelWidget.hpp"
#include "Utilities/TracingUtility.hpp"


RendererCenter::RendererCenter() : window(nullptr)
{
}

void RendererCenter::Init()
{
	InitSDL();

	CNAME;
	event_system.AddEvent(Event("OnRenderSceneDataReady",[&](TEventParams e) {
		auto clips = std::move(*e->Get<std::vector<RenderData>>("render_data"));
		RenderScene(clips);
	}));
	event_system.AddEvent(Event("OnRenderWidgetDataReady",[&](TEventParams e) {
		//std::vector<GCPtr<Widget>> clips = std::move(*e->Get<std::vector<GCPtr<Widget>>>("widget_data"));
		TStartF("UI渲染");
		auto clips = std::move(*e->Get<std::vector<RenderData>>("render_data"));
		//auto viewport = (*e->Get<GCPtr<GameWorld>>("widget_data"))->viewport;
		RenderScene(clips);
		TEndF("UI渲染");
	}));
	event_system.AddEvent(Event("OnRenderPresent",[&](TEventParams e) {
		TStartF("SDL_RenderPresent");
		SDL_RenderPresent(renderer);
		TEndF("SDL_RenderPresent");
	}));
	event_system.AddEvent(Event("OnRenderClear",[&](TEventParams e) {
		TStartF("SDL_RenderClear");
		SDL_SetRenderDrawColor(renderer, 100, 100, 100,0);
		SDL_RenderClear(renderer);
		TEndF("SDL_RenderClear");
	}));
	DefaultTexture = CreateOutLineTexture({100.f,100.f});
}

void RendererCenter::InitSDL()
{
	renderer = GameEngine::Instance().renderer;
	window = GameEngine::Instance().window;
}

void RendererCenter::DeInit()
{
	DeInitSDL();
}



SDL_Renderer* RendererCenter::GetRenderer() const
{
	return renderer;
}

void RendererCenter::SetRendererAndWindow(SDL_Renderer *r, SDL_Window *w)
{
	renderer = r;
	window = w;
}

void RendererCenter::RenderScene(std::vector<RenderData>& clips)
{
	//数据预处理
	TStart;
	std::ranges::sort(clips.begin(),clips.end(),[](const RenderData& A,const RenderData& B){ return A.layer > B.layer;});

	for (auto& clip : clips)
	{
		auto& [_, texture,trans,src_opt,dst_opt] = clip;
		auto point = trans.rotation.Point ? SDL_FPoint(trans.rotation.Point->x,trans.rotation.Point->y) : SDL_FPoint();
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
		//auto test = SDL_FRect(0,0,1,1);
		//SDL_RenderTexture(renderer,texture.get(),src,dst);
		SDL_RenderTextureRotated(renderer,texture.get(),src,dst,trans.rotation.Angle,&point,SDL_FLIP_NONE);
	}
	TEnd;
}

void RendererCenter::RenderWidget(std::vector<RenderData>& clips)
{
	std::ranges::sort(clips.begin(),clips.end(),[](const RenderData& A,const RenderData& B){ return A.layer > B.layer;});

	for (auto& clip : clips)
	{
		auto& [_, texture,trans,src_opt,dst_opt] = clip;
		auto point = trans.rotation.Point ? SDL_FPoint(trans.rotation.Point->x,trans.rotation.Point->y) : SDL_FPoint();
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
		//auto test = SDL_FRect(0,0,1,1);
		//SDL_RenderTexture(renderer,texture.get(),src,dst);
		SDL_RenderTextureRotated(renderer,texture.get(),src,dst,trans.rotation.Angle,&point,SDL_FLIP_NONE);
	}
}

std::shared_ptr<SDL_Texture> RendererCenter::CreateOutLineTexture(const Vec2<float>& size, SDL_Color color)
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

	int thickness = 2;
	for (int i = 0; i < thickness; ++i)
	{
		auto rect_bound = SDL_FRect(i,i, size.x - i * 2, size.y - i * 2);
		SDL_RenderRect(renderer, &rect_bound);
	}
	//切回屏幕
	SDL_SetRenderTarget(renderer,nullptr);
	return TTexture(t);
}

std::shared_ptr<SDL_Texture> RendererCenter::CreateFilledTexture(const Vec2<float> size, SDL_Color color)
{
	SDL_Texture* texture_T = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		size.x,
		size.y
	);
	SDL_SetRenderTarget(renderer, texture_T);
	SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
	const auto rect_T = SDL_FRect(0, 0, size.x, size.y);
	SDL_RenderFillRect(renderer,&rect_T);
	SDL_SetRenderTarget(renderer, nullptr);
	return TTexture(texture_T);
}

void RendererCenter::SetTextureFromSurface(Texture* t, std::shared_ptr<SDL_Surface> new_surface)
{
	auto s = new_surface.get();
	auto new_texture = SDL_CreateTextureFromSurface(renderer, s);
	auto new_texture_s = std::shared_ptr<SDL_Texture>(new_texture,SDLTextureDeleter());
	t->SetTexture(new_texture_s);
}



void RendererCenter::DeInitSDL() const
{

}

std::shared_ptr<SDL_Texture> RendererCenter::GetDefaultTexture()
{
	return DefaultTexture;
}

void SetTextureFromSurface_S(Texture *t, std::shared_ptr<SDL_Surface> new_surface)
{
	GameEngine::Instance().renderer_center->SetTextureFromSurface(t,new_surface);
}

// SDL_Texture * Create_OutLineTexture_S()
//
// {
// 	return GameEngine::Instance().renderer_center->Create
// }


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
