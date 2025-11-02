#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

#include "Classes/Core/GameWorld.hpp"

static int global_physics_id = 0;
void SPhysicsBaseUtility::SetPhysicsCallback(const std::function<void()>& physics_callback_)
{
	physics_callback = physics_callback_;
	quality = 0.046f * *GameEngine::Instance().GetEngineSubSystemManager()->GetSubSystem<RandomUtility>("RandomUtility")->GetRandom("SPhysicsBaseUtility_quality");
	if (quality == 0)
	{
		std::cout << "有问题 " << quality << std::endl;
	}
}

SPhysicsBaseUtility::SPhysicsBaseUtility()
{
	CNAME;
}

void SPhysicsBaseUtility::Construct()
{
	Object::Construct();
	//GameEngine::Instance().physicsSys->Register(this);
	dispatcher_system.AddEventDispatcher("OnCollision");//碰撞事件
	dispatcher_system.AddEventDispatcher("OnSynchronization");//同步事件

	//默认不订阅碰撞
	is_subscribe_collision = false;
}

SPhysicsBaseUtility::~SPhysicsBaseUtility()
{
	if (simulation_physics)
	{
		Engine().physicsSys->DeRegister(this);
	}
}

FRect SPhysicsBaseUtility::GetCollisionBox() const
{
	Location loc = collision_owner->GetComponentWorldLocation();
	Vec2<float> size = collision_owner->GetComponentSize();
    return {loc.x,loc.y,size.x,size.y};
}

void SPhysicsBaseUtility::SetSimulationPhysics(const bool is_active, const PhysicsType new_type)
{
	//修改物理类型
	type = new_type;

	//是否需要开关物理特性
	if (is_active == simulation_physics)
		return; // 没变化直接走人

	simulation_physics = is_active;

	if (is_active)
		Engine().physicsSys->Register(this);
	else
		Engine().physicsSys->DeRegister(this);
}

void SPhysicsBaseUtility::SetPhysicsType(const PhysicsType new_type)
{
	type = new_type;
}

// void SPhysicsBaseUtility::Destroy()
// {
// 	Wo
// }

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

void SPhysicsBaseUtility::SetSubscribeCollision(const bool is_subscribe)
{
	is_subscribe_collision = is_subscribe;
}

void SPhysicsBaseUtility::SynchronizationTransform()
{
	// const auto loc = collision_owner->GetComponentTransform().location;
	// collision_rect.x = loc.x;
	// collision_rect.y = loc.y;
}

// void SPhysicsBaseUtility::SetBodyBox(const Vec2<float> size)
// {
// 	w = size.x;
// 	h = size.y;
// }

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
}
