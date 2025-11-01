#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

#include "Classes/Core/GameWorld.hpp"

void MovableUtility::AddVelocity(const Vec2<float> addition_velocity)
{
    velocity += addition_velocity;
}

PhysicsType MovableUtility::GetPhysicsType()
{
    return PhysicsType::Movable;
}

// void MovableUtility::HandleVelocity(float delta_time)
// {
// 	SPhysicsBaseUtility::HandleVelocity(delta_time);
// }

void MovableUtility::Init()
{
    SPhysicsBaseUtility::Init();
	CNAME;
}

void MovableUtility::SetCollisionBound(FRect box)
{

}
