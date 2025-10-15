#pragma once

#include "FPoint.hpp"
#include "var_type.hpp"
#include "Vec.hpp"

struct Rotation  : var_type
{
    double Angle;
    FPoint Point;
    Rotation() : Angle(0.0){}
    Rotation(Vec2<float> Point_, float Angle_) : Point(Point_), Angle(Angle_){}
    std::string str()
    {
        return "Rotation(" + std::to_string(Angle) + "),Point(" + Point.str();
    }
};
