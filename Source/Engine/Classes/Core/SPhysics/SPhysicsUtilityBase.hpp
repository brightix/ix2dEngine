#pragma once
#include "Types/Vec.hpp"

enum class PhysicsType
{
    Static,
    RigidBody,
    Movable
};

class SPhysicsUtilityBase
{
protected:
    Vec2d<float> velocity;
    float quality = 10.f;
    float force_attenuation = 1.0f;
public:
    SPhysicsUtilityBase();
    virtual ~SPhysicsUtilityBase(){}
    virtual PhysicsType GetPhysicsType()= 0;
    virtual void HandleVelocity(float delta_time){}
};