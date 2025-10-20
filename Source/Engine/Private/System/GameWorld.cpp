#include "../../Classes/Core/GameWorld.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "TestActor.h"
#include "Classes/Pawn.hpp"
#include "Classes/GameModeBase.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

GameWorld::GameWorld() : is_simulation(false), is_server(false) {}

void GameWorld::Construct()
{
	NAME;
	auto game = game_mode;
	is_server = true;

	world_subsystem = NewObject<SubSystemManager>(new SubSystemManager);

	tick_SubSystem = world_subsystem->CreateSubSystem<TickSubSystem>("TickSubSystem");
	tick_SubSystem->SetBufferType(2);
	tick_SubSystem->dispatcher_system.AddEventDispatcher("synchronization");
	//world_subsystem->ForAllSubSystemInit();
	dispatcher_system.AddEventDispatcher("EventBegin");
}

void GameWorld::StartSimulation()
{
	printf("---------------simulation---------------\n");
	game_mode = make_GCPtr<GameModeBase>(new GameModeBase());
	game_mode->EventBegin();

	// for (const auto& a : actors)
	// {
	// 	a->EventBegin();
	// }
	//GC_timer->Start();
	is_simulation = true;

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

std::vector<GCPtr<Actor>> &GameWorld::GetActors()
{
	return actors;
}

void GameWorld::RemoveActorByGCPtr(GCPtr<Actor>& actor)
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




