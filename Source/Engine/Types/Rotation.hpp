#pragma once

#include "FPoint.hpp"
#include "var_type.hpp"

struct Rotation
{
    float Angle;
    std::optional<FPoint> Point;
    Rotation(float Angle_ = 0.f, std::optional<FPoint> Point_ = std::nullopt) : Angle(Angle_), Point(Point_){}

	static float Normalize(float angle)
    {
    	angle = fmod(angle, 360.f);
    	if (angle < 0) angle += 360.f;
    	return angle;
    }

	std::string Str() const
    {
    	return "Rotation " + std::to_string(Angle) + ", Point " + (Point ? Point->Str() : "null");
    }
};
