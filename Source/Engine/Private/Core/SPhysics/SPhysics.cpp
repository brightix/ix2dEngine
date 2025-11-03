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

SPhysics::~SPhysics()
{
	BREAK;
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
	TStartF("HandlePhysics");
    for (auto& unit : units)
    {
    	collision_tree.Insert(unit);
    	//静态对象不受物理状态影响
    	if (unit->type == PhysicsType::Static)
    	{
    		continue;
    	}
    	HandlePhysics(delta_time,unit);
    }
	TEndF("HandlePhysics");
	TCollisionPairs pairs;
    collision_tree.Query(pairs);

	TStartF("DoExtrusion");
	for (auto& [A,col_objs] : pairs)
	{
		if (A->type == PhysicsType::Static)
			continue;
		for (auto& col : col_objs)
		{
			Vec2<float> force;

			if (col->type == PhysicsType::Static)
			{
				switch (A->GetCollisionBox().CollisionDir(col->GetCollisionBox()))
				{
					case TOP:
						break;
					case BOTTOM:

						A->added_force -= A->velocity;
						break;
					case LEFT:
						break;
					case RIGHT:
						break;
				default: ;
				}
			}
			//auto v = col->velocity * A->bounciness;
		}
	}
	TEndF("DoExtrusion");

	TStartF("DispatcherOnCollision");
	for (auto& [A,col_objs] : pairs)
	{
		//不感兴趣的对象就不广播了
		if (!A->is_subscribe_collision)
		{
			continue;
		}
		EventParams e;
		e.Add<std::unordered_set<SPhysicsBaseUtility*>>("collision_objects",col_objs);
		A->dispatcher_system.CallDispatcher("OnCollision",e);
	}
	TEndF("DispatcherOnCollision");
	TEnd;

	//鼠标点击事件

    // TODO 四叉树碰撞
}

void SPhysics::HandlePhysics(const double delta_time, SPhysicsBaseUtility* unit) const
{
	float acceleration = world_physics.GravityForce / unit->quality;
	unit->velocity.y -= acceleration * delta_time;

	//处理冲量 可能来自 其他物体 或 主观
	unit->velocity += unit->added_force;	unit->added_force.Reset();	//消费
	//STOP_IF(unit->velocity.Length() < 1)
	//作用
	;
	unit->after_location = (unit->collision_owner->GetComponentWorldLocation() + unit->velocity * delta_time).Cast<float>();
}

void SPhysics::Synchronization() const
{
	TStart;
	for (auto& unit : units)
	{
		if (unit->type == PhysicsType::Static)
			continue;
		unit->dispatcher_system.CallDispatcher("OnSynchronization");
		// unit->collision_owner->SetComponentWorldLocation(unit->after_location);
		// unit->physics_callback();
	}
	TEnd;
}


void SPhysics::OnRigidCollision(SPhysicsBaseUtility *A, SPhysicsBaseUtility *B)
{
//两个物体互相施加反作用力，
	//std::cout << "碰撞了" << std::endl;
}
