#pragma once
#include "Vec.hpp"
struct EngineState
{
    float FPS = 0.0f;
    float DeltaTime = 0.0f;
	Vec2<int> ScreenSize;
};