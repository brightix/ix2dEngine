#pragma once
#include "var_type.hpp"
#include "Vec.hpp"

class FPoint : var_type
{
    Vec2d<float> point;
public:
    FPoint() : point(0.f,0.f){}
    FPoint(Vec2d<float> p) : point(p) {};
    std::string str() override
    {
        return std::to_string(point.x) + "," + std::to_string(point.y);
    }
};
