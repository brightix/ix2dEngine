#pragma once
#include "var_type.hpp"

struct Scaling
{
    float horizontal;
    float vertical;
    Scaling() : horizontal(1.0f), vertical(1.0f) {}
    Scaling(const float hor, const float vert) : horizontal(hor), vertical(vert) {}
    std::string Str()
    {
        return "Scaling: " + std::to_string(horizontal) + " " + std::to_string(vertical);
    }
};
