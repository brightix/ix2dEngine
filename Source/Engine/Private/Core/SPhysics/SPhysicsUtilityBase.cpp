#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

#include "Classes/Core/GameWorld.hpp"

SPhysicsBaseUtility::SPhysicsBaseUtility()
{
    SPhysicsBaseUtility::Init();
}

FRect SPhysicsBaseUtility::GetCollisionBox() const
{
    return collision_box;
}

void SPhysicsBaseUtility::SetIsSimulatedPhysics(bool value)
{
    is_simulated_physics = value;
}

void SPhysicsBaseUtility::Init()
{
    GetWorld()->physicsSys.Register(this);
}