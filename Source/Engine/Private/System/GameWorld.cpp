#include "System/GameWorld.hpp"
#include "System/GameEngine.hpp"
#include "TestActor.h"

GameWorld::GameWorld()
{
	game_mode = SpawnActorFromClass<GameModeBase>();
}

void GameWorld::Construct()
{
	Actor a(Transform(Vec2d<double>(50.0,50.0),{}));
	GCPtr<TestActor> c = make_GCPtr<TestActor>(1);
	GameEngine::Instance().GetGameWorld()->SpawnActorFromClass<TestActor>(1);
}

void GameWorld::PrintString(std::string, int exist_time, SDL_Color color)
{
	//debug_viewport->AddChild();
}

void GameWorld::Tick(double deltaTime)
{
	game_mode->Tick(deltaTime);
	for (auto& a : Actors) {
		a->Tick(deltaTime);
	}
}
