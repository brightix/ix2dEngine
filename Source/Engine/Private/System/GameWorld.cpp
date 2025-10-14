#include "../../Classes/Core/GameWorld.hpp"
#include "../../Classes/Core/GameEngine.hpp"
#include "TestActor.h"
#include "Classes/Pawn.hpp"
#include "Classes/SubSystem/Sub/GarbageCollection.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"
GameWorld::GameWorld() : is_simulation(false), is_server(false) {}

void GameWorld::Construct()
{
	auto game = game_mode;
	is_server = true;
	CreateWorldSubSystem<GarbageCollection>("GarbageCollection");
	//Actor a(Transform(Vec2d<float>(50.0,50.0),{}));
	//actors.push_back(make_GCPtr<Pawn>(new Pawn()));
	//GCPtr<TestActor> c = make_GCPtr<TestActor>(1);
	// auto g = SpawnActorFromClass<TestActor>(new TestActor(1));
	// auto g2 = SpawnActorFromClass<TestActor>(1);
	timer_system.SetTimer(2000,[this]() {
		std::cout << "{ " << dynamic_cast<GarbageCollection*>(GetWorldSubSystem("GarbageCollection"))->GCSweep() << " } objects have been Swept!" << std::endl;
		return 2000;
	});
}

void GameWorld::StartSimulation()
{
	printf("---------------simulation---------------\n");
	game_mode = make_GCPtr<GameModeBase>(new GameModeBase());
	game_mode->EventBegin();

	for (const auto& a : actors)
	{
		a->EventBegin();
	}
	//GC_timer->Start();
	is_simulation = true;


	auto dd = SpawnActor(new Actor(Transform{{500,500}}));
	GameEngine::Instance().timer_system.SetTimer(1000,[dd]() {
		dd->DestroyActor();
		return -1;
	});
}

std::vector<GCPtr<Controller>> GameWorld::GetControllers()
{
	return controllers;
}

std::vector<Actor *> GameWorld::GetActors()
{
	return actors;
}

void GameWorld::RemoveActorByPtr(Actor *actor)
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

void GameWorld::AddToWorld(Actor* actor)
{
	actors.push_back(actor);
}

void GameWorld::PrintString(std::string, int exist_time, SDL_Color color)
{
	//debug_viewport->AddChild();
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
	tick_manager.Tick(delta_time);
}

void GameWorld::WorldDestroy()
{
	for (auto& it : world_subSystem)
	{
		it.second->DeInit();
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




