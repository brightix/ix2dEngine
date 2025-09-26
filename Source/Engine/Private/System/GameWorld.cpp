#include "System/GameWorld.hpp"
#include "System/GameEngine.hpp"
#include "TestActor.h"
#include "Classes/Pawn.hpp"

GameWorld::GameWorld() : is_simulation(false) {}

void GameWorld::Construct()
{
	auto game = game_mode;
	Actor a(Transform(Vec2d<float>(50.0,50.0),{}));
	//actors.push_back(make_GCPtr<Pawn>(new Pawn()));
	//GCPtr<TestActor> c = make_GCPtr<TestActor>(1);
	auto g = SpawnActorFromClass<TestActor>(new TestActor(1));
	auto g2 = SpawnActorFromClass<TestActor>(1);

}

void GameWorld::StartSimulation()
{
	game_mode = make_GCPtr<GameModeBase>(new GameModeBase());
	game_mode->EventBegin();
	for (const auto& a : actors)
	{
		a->EventBegin();
	}
	//GC_timer->Start();
	is_simulation = true;
}

void GameWorld::AddToWorld(Actor* actor)
{
	actors.push_back(actor);
}

void GameWorld::PrintString(std::string, int exist_time, SDL_Color color)
{
	//debug_viewport->AddChild();
}

void GameWorld::Tick(double deltaTime)
{
	if (!is_simulation)
	{
		return;
	}
	//计算

	game_mode->Tick(deltaTime);
	for (auto& a : actors) {
		a->Tick(deltaTime);
	}
	//渲染
	for (auto& a : actors) {
		a->RenderOnScreen();
	}
}
