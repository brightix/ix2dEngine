#include "System/GameWorld.hpp"
#include "System/GameEngine.hpp"
#include "TestActor.h"
#include "Classes/Pawn.hpp"

GameWorld::GameWorld()
{
	game_mode = make_GCPtr<GameModeBase>(new GameModeBase());
	auto game = game_mode;
}

void GameWorld::Construct()
{
	Actor a(Transform(Vec2d<float>(50.0,50.0),{}));
	//actors.push_back(make_GCPtr<Pawn>(new Pawn()));
	//GCPtr<TestActor> c = make_GCPtr<TestActor>(1);
	auto g = GameEngine::Instance().GetGameWorld()->SpawnActorFromClass<TestActor>(1);
}

void GameWorld::ConstructWorld()
{
	for (auto& a : actors)
	{
		a->EventBegin();
	}
}

void GameWorld::AddToWorld(GCPtr<Actor> actor)
{
	actors.push_back(actor);
}

void GameWorld::PrintString(std::string, int exist_time, SDL_Color color)
{
	//debug_viewport->AddChild();
}

void GameWorld::Tick(double deltaTime)
{
	game_mode->Tick(deltaTime);
	for (auto& a : actors) {
		a->Tick(deltaTime);
	}
}
