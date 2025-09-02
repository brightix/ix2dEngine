#pragma once
#include <Vec.hpp>
#include <Rotation.hpp>
struct Transform
{
    Vec2d<double> Location;
    Rotation rotation;
    
    Transform() : rotation({}),Location({}) {}
    Transform(Vec2d<double> Location_,Rotation rotation_) : Location(Location_),rotation(rotation_) {}
};