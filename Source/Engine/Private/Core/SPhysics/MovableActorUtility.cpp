#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

void MovableActorUtility::AddVelocity(const Vec2<float> addition_velocity)
{
    velocity += addition_velocity;
}

PhysicsType MovableActorUtility::GetPhysicsType()
{
    return PhysicsType::Movable;
}

void MovableActorUtility::HandleVelocity(float delta_time)
{
	SPhysicsUtilityBase::HandleVelocity(delta_time);
}
