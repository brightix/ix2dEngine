#pragma once
#include "Rotation.hpp"
#include "var_type.hpp"
#include "Vec.hpp"

#include "Utilities/FuncLib/GlobalMacros.hpp"
struct Location : var_type
{
    float x;
    float y;
    Location() : x(0), y(0) {}
    Location(const Vec2<float> l) : x(l.x), y(l.y) {}

	void RotateByAngle(float angle, Vec2<float> point)
    {
		auto dx = x - point.x;
    	auto dy = y - point.y;
    	float x_new = dx * cos(PI) - dy * sin(PI);
    	float y_new = dx * sin(PI) + dy * cos(PI);
    	x += x_new;
    	y += y_new;
    }






    std::string str() override
    {
        return "x: " + std::to_string(x) + ", y: " + std::to_string(y);
    }




    void operator+=(const Vec2<float> l)
    {
        x += l.x;
        y += l.y;
    }
	Vec2<float> operator+(Location& other) const
	{

	    return Vec2(x+other.x,y+other.y);
    }
	Vec2<float> operator-(Location& other) const
    {

    	return Vec2(x-other.x,y-other.y);
    }
};
