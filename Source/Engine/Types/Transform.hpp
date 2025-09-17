#pragma once

#include "Rotation.hpp"
#include "Types/Vec.hpp"

struct Transform
{
    Vec2d<float> Location;
    Rotation rotation;
    
    Transform() : rotation({}),Location({}) {}
    Transform(const Vec2d<float> Location_, const Rotation rotation_) : Location(Location_),rotation(rotation_) {}
};
