#pragma once 
#include <Vec.hpp>
class Scale
{
    Vec2d<float> Scaling;
    Scale() : Scaling({}){}
    Scale(float x, float y) : Scaling({x,y}){}
    Scale(Vec2d<float> Scaling_) : Scaling(Scaling_){}
};