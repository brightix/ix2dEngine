#pragma once
#include "var_type.hpp"
#include "Vec.hpp"
#include "Classes/Actor.hpp"

struct Location : var_type
{
    float x;
    float y;
    Location() : x(0), y(0) {}
    explicit Location(const Vec2d<float> l) : x(l.x), y(l.y) {}
    std::string str() override
    {
        return "x: " + std::to_string(x) + ", y: " + std::to_string(y);
    }
    void operator+=(const Vec2d<float> l)
    {
        x += l.x;
        y += l.y;
    }
};
