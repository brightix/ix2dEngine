#pragma once
#include "Vec.hpp"
struct EngineState
{
    float FPS = -1.f;
    float DeltaTime = -1.f;
	Vec2<int> ScreenSize;
};