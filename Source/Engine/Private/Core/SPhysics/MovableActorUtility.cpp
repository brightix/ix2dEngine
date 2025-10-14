#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

void MovableActorUtility::AddVelocity(const Vec2d<float> addition_velocity)
{
    velocity += addition_velocity;
}

PhysicsType MovableActorUtility::GetPhysicsType()
{
    return PhysicsType::Movable;
}
