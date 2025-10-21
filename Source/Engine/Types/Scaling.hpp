#pragma once
#include "var_type.hpp"

struct Scaling : var_type
{
    float horizontal;
    float vertical;
    Scaling() : horizontal(1.0f), vertical(1.0f) {}
    Scaling(const float hor, const float vert) : horizontal(hor), vertical(vert) {}
    std::string str()
    {
        return "Scaling: " + std::to_string(horizontal) + " " + std::to_string(vertical);
    }
};
