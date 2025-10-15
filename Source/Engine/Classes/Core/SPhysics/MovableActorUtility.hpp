#pragma once
#include "SPhysicsUtilityBase.hpp"
#include "Types/Vec.hpp"


class MovableUtility : public SPhysicsBaseUtility
{
public:
    void AddVelocity(const Vec2<float> addition_velocity);
    PhysicsType GetPhysicsType() override;
    void HandleVelocity(float delta_time) override;
    void Init() override;
    void SetCollisionBound(FRect box);
};
