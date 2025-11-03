#pragma once
#include "Vec.hpp"
struct EngineState
{
    float FPS = 0.0f;
    double DeltaTime = 0.0;
	Vec2<int> ScreenSize;
};