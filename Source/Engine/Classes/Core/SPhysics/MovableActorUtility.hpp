#pragma once
#include "SPhysicsUtilityBase.hpp"
#include "Types/Vec.hpp"


class MovableActorUtility final : public SPhysicsUtilityBase
{
public:
    void AddVelocity(const Vec2<float> addition_velocity);
    PhysicsType GetPhysicsType() override;
    void HandleVelocity(float delta_time) override;
};
