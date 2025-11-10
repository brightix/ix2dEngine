#include "Classes/Core/TickManager.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Controller.hpp"
#include "Types/RenderData.hpp"
#include "Utilities/TracingUtility.hpp"

TickSubSystem::TickSubSystem(int buffer_cnt) : buffer_type(buffer_cnt), fence(buffer_cnt)
{
	CNAME;
}

void TickSubSystem::Construct()
{
	EngineSubSystem::Construct();
	show_debug_line = false;
}

void TickSubSystem::RegisterEvents()
{
	EngineSubSystem::RegisterEvents();
	AddCustomEvent(Event("ShowDebugLine",[this](bool is_show) {
		show_debug_line = is_show;
	}));
}

void TickSubSystem::RegisterDispatchers()
{
	EngineSubSystem::RegisterDispatchers();
	AddDispatcher("OnRenderClear");
	AddDispatcher("OnRenderPresent");
	AddDispatcher("OnRenderSceneDataReady", {typeid(std::vector<RenderData>)});
	AddDispatcher("OnRenderWidgetDataReady",{typeid(std::vector<RenderData>)});
}

void TickSubSystem::Tick(const double delta_time)
{
TStart;

TStartF("TickSubInit");
	GameWorld* world = World();
    const auto actors = world->GetActors();
    const std::vector<Controller*> controllers = world->GetControllers();
	auto& physics = Engine().physicsSys;
	std::vector<RenderData> render_data;
	render_data.reserve(actors.size());
TEndF("TickSubInit");

	if (buffer_type == 1)
	{
		NewTimer timer;
		//计算物理

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


		physics->SimulationTunneling(delta_time);

TStartF("SetTexts");
		texts[0]->SetText("TickDelay: " + std::to_string(timer.Click()));
TEndF("SetTexts");
TStartF("viewportTick");
		world->viewport->ForTick(delta_time);
TEndF("viewportTick");

		texts[1]->SetText("WidgetTickDelay: " + std::to_string(timer.Click()));

		physics->Synchronization();

		//渲染纹理
TStartF("收集渲染数据");
		for (auto& a : actors)
		{
			if (a->is_pre_kill)
			{
				a->is_pending_kill = true;
				world->RemoveActor(a);
			}
			else if (a->IsActive() && a->IsVisible())
			{
				//a->RenderOnScreen();
				a->ForRenderOrder(render_data);
			}
		}
		//控件
		world->viewport->NativeOfferRenderData(render_data);
TEndF("收集渲染数据");

		//清屏

		CallDispatcher("OnRenderClear");


		texts[2]->SetText("SceneRenderOfferDelay: " + std::to_string(timer.Click()));
		//场景组件
		// EventParams render_data_ready_p;
		// render_data_ready_p.Add<std::vector<RenderData>>("render_data",std::move(render_data));
		CallDispatcher("OnRenderSceneDataReady",std::move(render_data));

		texts[3]->SetText("AllPhysicsBody count: " + std::to_string(Engine().physicsSys->units.size()));




		//Widget
		// EventParams widget_data;
		// widget_data.Add<GCPtr<GameWorld>>("widget_data", world);
		// dispatcher_system.CallDelegate("RenderWidgetDataReady", widget_data);
		if (show_debug_line)
		{
			physics->DebugTree();
		}
// 显示到窗口  停止提交任务 ##################################################################################

		dispatcher_system.CallDispatcher("OnRenderPresent");

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
	texts.resize(4);
	for (int i = 0; i < 4; i++)
	{
		const auto text = CreateWidget<TextBlockWidget>();
		AddToViewport(text)->display_area.y = static_cast<float>(3 + i) * 26;

		texts[i] = text;
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
// 	dispatcher_system.CallDelegate("synchronization");
//
//
// }