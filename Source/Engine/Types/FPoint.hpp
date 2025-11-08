#pragma once
#include "var_type.hpp"
#include "Vec.hpp"

struct FPoint
{
    float x;
	float y;
    FPoint() : x(0.f), y(0.f) {}
	FPoint(float _x, float _y) : x(_x), y(_y) {}
    FPoint(Vec2<float> p)  : x(p.x), y(p.y) {};
    std::string Str() const
    {
        return std::to_string(x) + "," + std::to_string(y);
    }
};
