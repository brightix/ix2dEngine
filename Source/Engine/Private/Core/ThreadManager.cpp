#include "Classes/Core/ThreadManager.hpp"

#include <thread>
#include <SDL3/SDL_render.h>

#include "../../Classes/Core/GameEngine.hpp"

ThreadManager::ThreadManager() 
{
    
}

void ThreadManager::StartRenderThread()
{
	std::thread([this]() {
		SDL_Renderer* renderer = GameEngine::Instance().GetRenderer();
		while (!is_stop.load())
		{
			std::unique_lock lock(render_mtx);
			//等待第二帧数据准备完成
			render_cv.wait(lock, [this] {
				return render_fence[(render_idx+1) % 2].load();
			});
			//auto& [actors,transforms,sprites,source_rects] = render_buffer_shot[render_idx];
			// int n = actors.size();
			// for (int i = 0; i < n; i++)
			// {
			// 	actors[i]->GetPivot();
			// 	SDL_RenderTexture(renderer,sprites[i].GetTexture(),);
			// }
			//
			// //前一帧消费完成 标记可写入
			// render_fence[render_idx].store(false);
		}
	});
}
