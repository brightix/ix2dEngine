#include "Classes/Core/TickManager.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Controller.hpp"
#include "Types/RenderData.hpp"
#include "Utilities/TracingUtility.hpp"

void TickSubSystem::Tick(double delta_time)
{
	TStart;

	TStartF("TickSubInit");
    auto& actors = *GameEngine::Instance().GetGameWorld()->GetActors();
    std::vector<GCPtr<Controller>> controllers = GameEngine::Instance().GetGameWorld()->GetControllers();
    auto world = GameEngine::Instance().GetGameWorld();
	auto& physics = world->physicsSys;
	std::vector<RenderData> render_data;
	render_data.reserve(actors.size());
	TEndF("TickSubInit");

	if (buffer_type == 1)
	{
		NewTimer timer;
		//计算物理
		physics.simulation(delta_time);

		//普通Tick
		timer.Start();
		TStartF("NormalTick");
		 for (auto& a : actors)
		 {
		 	if (a->IsActive())
		 	{
		 		a->Tick(delta_time);
		 		a->RootComponentTick(delta_time);
		 	}
		 }
		TEndF("NormalTick");

		TStartF("SetTexts");
		texts[0]->SetText("TickDelay: " + std::to_string(timer.Click()));
		TEndF("SetTexts");

		TStartF("viewportTick");
		world->viewport->ForTick(delta_time);
		TEndF("viewportTick");


		texts[1]->SetText("WidgetTickDelay: " + std::to_string(timer.Click()));
		//渲染纹理

		TStartF("收集渲染数据");
		for (auto& a : actors)
		{
			if (a->is_pre_kill)
			{
				a->is_pending_kill = true;
				world->RemoveActorByGCPtr(a);
			}
			else if (a->IsActive() && a->IsVisible())
			{
				//a->RenderOnScreen();
				a->ForRenderOrder(render_data);
			}
		}
		TEndF("收集渲染数据");

		//清屏

		dispatcher_system.CallDispatcher("RenderClear");


		texts[2]->SetText("SceneRenderOfferDelay: " + std::to_string(timer.Click()));
		//场景组件
		EventParams render_data_ready_p;
		render_data_ready_p.Add<std::vector<RenderData>>("render_data",std::move(render_data));
		dispatcher_system.CallDispatcher("RenderSceneDataReady",std::move(render_data_ready_p));

		texts[3]->SetText("SceneRenderDelay: " + std::to_string(timer.Click()));


		//Widget
		EventParams widget_data;
		widget_data.Add<GCWeakPtr<GameWorld>>("widget_data", world);
		dispatcher_system.CallDispatcher("RenderWidgetDataReady", widget_data);

		physics.DebugTree();
// 显示到窗口  停止提交任务 ##################################################################################

		dispatcher_system.CallDispatcher("RenderPresent");

		physics.Synchronization();


		TStartF("SDL_ControllerTick");
		for (auto& controller : controllers)
		{
			controller->Tick(delta_time);
		}
		TEndF("SDL_ControllerTick");
	}
	TEnd;
}

void TickSubSystem::Init()
{
	CNAME;
	dispatcher_system.AddEventDispatcher("RenderClear");
	dispatcher_system.AddEventDispatcher("RenderPresent");
	dispatcher_system.AddEventDispatcher("RenderSceneDataReady");
	dispatcher_system.AddEventDispatcher("RenderWidgetDataReady");

	texts.resize(4);
	for (int i = 0; i < 4; i++)
	{
		auto text = CreateWidget(new TextBlockWidget);
		auto slot = AddToViewport(text);
		slot->display_area.y = (i+3)*26;
		texts[i] = slot->widget;
	}
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