#pragma once
#include "Types/Vec.hpp"

struct PivotDir
{
	constexpr static Vec2<float> BOTTOM_CENTER = {0.5f, 1.0f};
	constexpr static Vec2<float> TOP_CENTER    = {0.5f, 0.0f};
	constexpr static Vec2<float> CENTER       = {0.5f, 0.5f};
	constexpr static Vec2<float> LEFT_CENTER  = {0.0f, 0.5f};
	constexpr static Vec2<float> RIGHT_CENTER = {1.0f, 0.5f};
};

