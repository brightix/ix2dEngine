#pragma once
#include "Location.hpp"
#include "Rotation.hpp"
#include "Scaling.hpp"
#include "var_type.hpp"
#include "Vec.hpp"

struct Transform : var_type
{
    Location location;
    Rotation rotation;
    Scaling scaling;
    Transform() {}
    explicit Transform(const Vec2d<float> loc) : location(loc){}
    Transform(const Vec2d<float> Location_, const Rotation rotation_) : location(Location_),rotation(rotation_) {}

    std::string str() override
    {
        return {};
    }
};
