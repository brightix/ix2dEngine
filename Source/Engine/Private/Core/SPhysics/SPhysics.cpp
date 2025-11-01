#include "Classes/Core/SPhysics/SPhysics.hpp"

#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"
#include "Utilities/QuadTree.hpp"

SPhysics::SPhysics()
{
    collision_tree.SetCollisionStrategy([](SPhysicsBaseUtility *A, SPhysicsBaseUtility *B) {
        OnRigidCollision(A, B);
    });
	auto size = GameEngine::Instance().GetEngineAttribution().ScreenSize;
	collision_tree.SetBoundary({0,0,size.x,size.y});
	collision_tree.Clear();
}

void SPhysics::Register(SPhysicsBaseUtility* unit)
{
    units.insert(unit);
}

void SPhysics::DeRegister(SPhysicsBaseUtility* unit)
{
	units.erase(unit);
}

void SPhysics::simulation(const double delta_time)//注意tunneling，分批tick
{
	TStart;
    collision_tree.Clear();
    for (auto& unit : units)
    {
    	//unit->HandleVelocity(delta_time);

    	collision_tree.Insert(unit);
    	if (unit->type == PhysicsType::Static)
    	{
    		continue;
    	}
    	HandlePhysics(delta_time,unit);
    }
	TCollisionPairs pairs;
    collision_tree.Query(pairs);
	for (auto& [A,col_objs] : pairs)
	{
		if (A->type == PhysicsType::Static)
		{
			continue;
		}
		EventParams e;
		e.Add<std::unordered_set<SPhysicsBaseUtility*>>("collision_objects",col_objs);
		A->dispatcher_system.CallDispatcher("OnCollision",e);
	}
	TEnd;

	//鼠标点击事件

    // TODO 四叉树碰撞
}

void SPhysics::HandlePhysics(const double delta_time, SPhysicsBaseUtility* unit) const
{
	unit->velocity.y -= world_physics.GravityForce * delta_time * unit->quality;

	//处理冲量 可能来自 其他物体 或 主观
	unit->velocity += unit->added_force;	unit->added_force.Reset();	//消费

	//作用
	unit->after_location = unit->collision_owner->GetComponentWorldLocation() + unit->velocity;



}

void SPhysics::Synchronization() const
{
	for (auto& unit : units)
	{
		if (unit->type == PhysicsType::Static)
			continue;

		unit->collision_owner->SetComponentWorldLocation(unit->after_location);
		unit->physics_callback();
	}
}


void SPhysics::OnRigidCollision(SPhysicsBaseUtility *A, SPhysicsBaseUtility *B)
{
//两个物体互相施加反作用力，
	//std::cout << "碰撞了" << std::endl;
}
