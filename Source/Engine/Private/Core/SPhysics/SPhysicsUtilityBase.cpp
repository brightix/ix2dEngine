#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

#include "Classes/Core/GameWorld.hpp"

static int global_physics_id = 0;
SPhysicsBaseUtility::SPhysicsBaseUtility()
{
	NAME;
	SPhysicsBaseUtility::Init();

	test_name = std::to_string(global_physics_id++);
}

SPhysicsBaseUtility::~SPhysicsBaseUtility()
{
	World()->physicsSys.DeRegister(this);
}

FRect SPhysicsBaseUtility::GetCollisionBox() const
{
    return collision_box;
}

void SPhysicsBaseUtility::SetIsSimulatedPhysics(bool value)
{
    is_simulated_physics = value;
}

void SPhysicsBaseUtility::SetOwner(SceneComponent* new_owner)
{
	owner = new_owner;
}

void SPhysicsBaseUtility::SetBodyWorldLocation(const Location& location)
{
	collision_box.x = location.x;
	collision_box.y = location.y;
}

// TODO
void SPhysicsBaseUtility::SetBodyWorldRotation(const Rotation& rotation) { }

void SPhysicsBaseUtility::AddBodyWorldLocation(Vec2<float> v)
{
	collision_box.x += v.x;
	collision_box.y += v.y;
}

void SPhysicsBaseUtility::Init()
{
    GetWorld()->physicsSys.Register(this);
}
