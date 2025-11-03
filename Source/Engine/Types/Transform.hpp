#pragma once
#include "Rotation.hpp"
#include "Scaling.hpp"

struct Transform final : var_type
{
    Vec2<float> location;
    Rotation rotation;
    Scaling scaling;
    Transform() = default;
    explicit Transform(const Vec2<float> &loc) : location(loc){}
    Transform(const Vec2<float> &Location_, const Rotation &rotation_) : location(Location_),rotation(rotation_) {}

    std::string str() override
    {
        return {};
    }
};
