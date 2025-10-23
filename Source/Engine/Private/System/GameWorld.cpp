#include "../../Classes/Core/GameWorld.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "public/TestActor.h"
#include "Classes/GameModeBase.hpp"
#include "Classes/SubSystem/ViewportSubSystem.hpp"
#include "Public/TestFpsWidget.hpp"

GameWorld::GameWorld() : is_simulation(false), is_server(false) {}

void GameWorld::Construct()
{
	NAME;
	auto game = game_mode;
	is_server = true;

	world_subsystem = NewObject<SubSystemManager>(new SubSystemManager);
	//窗口
	viewport_sub = world_subsystem->CreateSubSystem<ViewportSubSystem>("ViewportSubSystem");
	//tick管理器
	tick_SubSystem = world_subsystem->CreateSubSystem<TickSubSystem>("TickSubSystem");
	tick_SubSystem->SetBufferType(1);
	tick_SubSystem->dispatcher_system.AddEventDispatcher("synchronization");//可以加在该子系统的构造函数内

	//world_subsystem->ForAllSubSystemInit();




	dispatcher_system.AddEventDispatcher("EventBegin");
}

void GameWorld::StartSimulation()
{
	printf("---------------simulation---------------\n");
	game_mode = make_GCPtr<GameModeBase>(new GameModeBase());
	game_mode->EventBegin();
	is_simulation = true;


	fps_text = AddToViewport(new TestFpsWidget()).Cast<TestFpsWidget>();
	auto dd = SpawnActor(new Actor(Transform{{500,500}}));
	// GameEngine::Instance().timer_system.SetTimer(1000,[dd]() {
	// 	dd->DestroyActor();
	// 	return -1;
	// });

	dispatcher_system.CallDispatcher("EventBegin");
}

std::vector<GCPtr<Controller>> GameWorld::GetControllers()
{
	return controllers;
}

Controller * GameWorld::GetController(int id)
{
	if (id >= 0 && id < controllers.size())
	{
		return controllers[id].Get();
	}
	return nullptr;
}

std::vector<GCPtr<Actor>> *GameWorld::GetActors()
{
	return &actors;
}

std::vector<GCWeakPtr<Widget>> GameWorld::GetWidgets()
{
	std::vector<GCWeakPtr<Widget>> v;
	for (auto& item : widgets)
	{
		v.emplace_back(item);
	}
	return v;
}

void GameWorld::RemoveActorByGCPtr(const GCPtr<Actor>& actor)
{
	std::erase(actors, actor);
}

GCPtr<Controller> GameWorld::AddController(Controller *controller)
{
	if (is_server)
	{
		GCPtr<Controller> t = make_GCPtr<Controller>(controller);
		controllers.push_back(t);
		t->Construct();
		t->EventBegin();
		return t;
	}
	return {};
}

void GameWorld::AddToWorld(GCPtr<Actor> actor)
{
	actors.emplace_back(actor);
}

void GameWorld::Tick(double delta_time)
{
	if (!is_simulation)
	{
		return;
	}
	timer_system.Run();
	//一般情况下不使用game_mode的tick
	game_mode->Tick(delta_time);
	tick_SubSystem->Tick(delta_time);
}

void GameWorld::WorldDestroy()
{
	for (const auto subsystem = world_subsystem->GetAllSubSystem(); auto& it : subsystem)
	{
		if (const auto ret = it.Peek())
		{
			ret->DeInit();
		}
	}
}

bool GameWorld::IsServer() const
{
	return is_server;
}

bool GameWorld::IsClient() const
{
	return !is_server;
}

GCWeakPtr<Widget> GameWorld::AddToViewport(Widget* w)
{
	auto gc = NewObject(w);
	if (is_simulation)
	{
		w->WidgetEventBegin();
	}
	else
	{
		BindEvent(w, "EventBegin", Event([w](TEventParams e) {
			w->WidgetEventBegin();
		}));
	}
	widgets.emplace(gc);
	return gc;
}




