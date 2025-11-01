#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

#include "Classes/Core/GameWorld.hpp"

static int global_physics_id = 0;
void SPhysicsBaseUtility::SetPhysicsCallback(const std::function<void()>& physics_callback_)
{
	physics_callback = physics_callback_;
	quality = 0.046f * *GameEngine::Instance().GetEngineSubSystemManager()->GetSubSystem<RandomUtility>("RandomUtility")->GetRandom("SPhysicsBaseUtility_quality");
}

SPhysicsBaseUtility::SPhysicsBaseUtility()
{
	CNAME;
	SPhysicsBaseUtility::Init();
}

void SPhysicsBaseUtility::Construct()
{
	Object::Construct();
	dispatcher_system.AddEventDispatcher("OnCollision");
}

SPhysicsBaseUtility::~SPhysicsBaseUtility()
{
	World()->physicsSys.DeRegister(this);
}

FRect SPhysicsBaseUtility::GetCollisionBox() const
{
	Location loc = collision_owner->GetComponentWorldLocation();
    return FRect(loc.x,loc.y,w,h);
}

void SPhysicsBaseUtility::HandleVelocity(double delta_time)
{

}

// void SPhysicsBaseUtility::SetIsSimulatedPhysics(bool value)
// {
//     is_simulated_physics = value;
// }

void SPhysicsBaseUtility::SetOwner(SceneComponent* new_owner)
{
	collision_owner = new_owner;
}

void SPhysicsBaseUtility::SynchronizationTransform()
{
	// const auto loc = collision_owner->GetComponentTransform().location;
	// collision_rect.x = loc.x;
	// collision_rect.y = loc.y;
}

void SPhysicsBaseUtility::SetBodyBox(const Vec2<float> size)
{
	w = size.x;
	h = size.y;
}

void SPhysicsBaseUtility::SetBodyTransform(Transform transform)
{

}

void SPhysicsBaseUtility::SetBodyWorldLocation(const Location& location)
{
	// collision_rect.x = location.x;
	// collision_rect.y = location.y;
	std::cout << "调用了已删除的函数，物理组件的 设置 世界位置" << std::endl;
}

// TODO
void SPhysicsBaseUtility::SetBodyWorldRotation(const Rotation& rotation) { }

void SPhysicsBaseUtility::AddBodyWorldLocation(Vec2<float> v)
{
	// collision_rect.x += v.x;
	// collision_rect.y += v.y;
	std::cout << "调用了已删除的函数，物理组件的 添加 世界位置" << std::endl;
}

void SPhysicsBaseUtility::Init()
{
    GetWorld()->physicsSys.Register(this);
}
