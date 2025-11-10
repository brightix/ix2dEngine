#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

#include "Classes/Core/GameWorld.hpp"

static int global_physics_id = 0;

SPhysicsBaseUtility::SPhysicsBaseUtility()
{
	CNAME;
}

void SPhysicsBaseUtility::Construct()
{
	Object::Construct();
	//GameEngine::Instance().physicsSys->Register(this);
	type = PhysicsType::Static;
	mass_inv = 1.f / mass;
	//默认不订阅碰撞
	is_subscribe_collision = false;
}

void SPhysicsBaseUtility::RegisterDispatchers()
{
	Object::RegisterDispatchers();
	AddDispatcher("OnCollision",{TypeID(std::unordered_set<SPhysicsBaseUtility*>)});//碰撞事件
	AddDispatcher("OnSynchronization");//同步事件
}

void SPhysicsBaseUtility::NativeSetOuter(GCObject *new_outer)
{
	Object::NativeSetOuter(new_outer);
	if ((collision_owner = Cast<SceneComponent>(new_outer)))
	{
		name = collision_owner->name + "_physicsBody";
	}

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
	return collision_owner->GetComponentCollisionBoundary();
}

void SPhysicsBaseUtility::SetSimulationPhysics(const bool is_active)
{
	//修改物理类型
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

void SPhysicsBaseUtility::SetPhysicsBodyOwner(SceneComponent* new_owner)
{
	collision_owner = new_owner;
}

void SPhysicsBaseUtility::SetSubscribeCollision(const bool is_subscribe)
{
	is_subscribe_collision = is_subscribe;
}

void SPhysicsBaseUtility::AddImpulse(const Vec2<float> &force)
{
	added_force += force;
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

void SPhysicsBaseUtility::SetBodyWorldLocation(const Vec2<float>& location)
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
