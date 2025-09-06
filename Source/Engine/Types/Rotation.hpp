#pragma once

#include "Types/Vec.hpp"

struct Rotation 
{
    double Angle;
    Vec2d<float> Point;
    Rotation() : Point({}), Angle(0.0){}
    Rotation(Vec2d<float> Point_, float Angle_) : Point(Point_), Angle(Angle_){}
};
