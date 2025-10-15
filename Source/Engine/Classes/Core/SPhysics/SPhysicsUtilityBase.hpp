#pragma once
#include "Types/FRect.hpp"
#include "Types/Vec.hpp"

enum class PhysicsType
{
    Static,
    RigidBody,
    Movable
};

class SPhysicsBaseUtility
{
protected:
    FRect collision_box;
    Vec2<float> velocity;
    float quality = 10.f;
    float force_attenuation = 1.0f;
    bool is_simulated_physics;

public:
    SPhysicsBaseUtility();
    virtual ~SPhysicsBaseUtility(){}
    virtual PhysicsType GetPhysicsType()= 0;
    [[nodiscard]] FRect GetCollisionBox() const;
    virtual void HandleVelocity(float delta_time){}

    virtual void Init();
    void SetIsSimulatedPhysics(bool value);
};