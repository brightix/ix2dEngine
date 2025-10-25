#pragma once
#include "var_type.hpp"
#include "Vec.hpp"

struct FPoint final : var_type
{
    float x;
	float y;
    FPoint() : x(0.f), y(0.f) {}
    FPoint(Vec2<float> p)  : x(p.x), y(p.y) {};
    std::string str() override
    {
        return std::to_string(x) + "," + std::to_string(y);
    }
};
