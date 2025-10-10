#pragma once
#include "System/GameEngine.hpp"

static GameWorld* World()
{
	return GameEngine::Instance().GetGameWorld().Get();
}
