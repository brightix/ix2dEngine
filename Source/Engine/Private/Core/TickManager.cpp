#include "Classes/Core/TickManager.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Controller.hpp"
#include "Types/RenderData.hpp"

void TickSubSystem::Tick(double delta_time)
{
    auto& actors = *GameEngine::Instance().GetGameWorld()->GetActors();
    std::vector<GCPtr<Controller>> controllers = GameEngine::Instance().GetGameWorld()->GetControllers();
    auto world = GameEngine::Instance().GetGameWorld();

	std::vector<RenderData> render_data;
	render_data.reserve(actors.size());
	if (buffer_type == 1)
	{
		//计算物理
		world->physicsSys.simulation(delta_time);
		//映射物理
		for (auto& a : actors)
		{
			if (a->IsActive())
			{
				a->PostPhysicsTick(delta_time);
			}
		}
		//普通Tick
		for (auto& a : actors)
		{
			if (a->IsActive())
			{
				a->Tick(delta_time);
				a->RootComponentTick(delta_time);
			}
		}

		world->viewport->ForTick(delta_time);

		//渲染纹理
		for (auto& a : actors)
		{
			if (a->is_pre_kill)
			{
				a->is_pending_kill = true;
				world->RemoveActorByGCPtr(a);
			}
			else if (a->IsActive())
			{
				//a->RenderOnScreen();
				a->ForRenderOrder(render_data);
			}
		}

		//清屏
		dispatcher_system.CallDispatcher("RenderClear");

		//场景组件
		EventParams render_data_ready_p;
		render_data_ready_p.Add<std::vector<RenderData>>("render_data",std::move(render_data));
		dispatcher_system.CallDispatcher("RenderSceneDataReady", render_data_ready_p);


		//Widget
		EventParams widget_data;
		std::vector<GCWeakPtr<Widget>> v;
		/** 将widget有序的放入vector中 */
		widget_data.Add<GCWeakPtr<GameWorld>>("widget_data", world);
		dispatcher_system.CallDispatcher("RenderWidgetDataReady", widget_data);

		// 显示到窗口
		dispatcher_system.CallDispatcher("RenderPresent");
		SDL_RenderPresent(GetRenderer());


		for (auto& controller : controllers)
		{
			controller->Tick(delta_time);
		}
	}
}

void TickSubSystem::Init()
{
	NAME;
	dispatcher_system.AddEventDispatcher("RenderClear");
	dispatcher_system.AddEventDispatcher("RenderPresent");
	dispatcher_system.AddEventDispatcher("RenderSceneDataReady");
	dispatcher_system.AddEventDispatcher("RenderWidgetDataReady");
}


void TickSubSystem::SetBufferType(int type) { buffer_type = type; }

// else if (buffer_type == 2)//  双缓冲(好像对于SDL无意义，暂时作废)  ---------------------------------------------------------------------------------
// {
// 	//PreTick
//
// 	//普通Tick
// 	for (auto& a : actors)
// 	{
// 		if (a->IsActive())
// 		{
// 			a->Tick(delta_time);
// 			a->RootComponentTick(delta_time);
// 		}
// 	}
// 	//提交渲染线程命令
// 	for (auto& a : actors)
// 	{
// 		if (a->is_pre_kill)//
// 		{
// 			a->is_pending_kill = true;
// 			world->RemoveActorByGCPtr(a);
// 		}
// 		else if (a->IsVisible())
// 		{
// 			a->ForRenderOrder(render_data);
// 		}
// 	}
//
// 	for (auto& controller : controllers)
// 	{
// 		controller->Tick(delta_time);
// 	}
//
//
// 	dispatcher_system.CallDispatcher("synchronization");
//
//
// }