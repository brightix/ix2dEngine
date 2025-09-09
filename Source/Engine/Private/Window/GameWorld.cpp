#include "Window/GameWorld.hpp"

GameWorld::GameWorld() 
{
	game_mode = SpawnActorFromClass<GameModeBase>();
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
