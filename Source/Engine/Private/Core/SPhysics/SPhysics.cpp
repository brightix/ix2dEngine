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
}

void SPhysics::Register(SPhysicsBaseUtility* unit)
{
    units.insert(unit);
}

void SPhysics::DeRegister(SPhysicsBaseUtility* unit)
{
	units.erase(unit);
}


void SPhysics::SimulationTunneling(const double delta_time)
{
	static constexpr double per_tunneling = 1.0 / 30.0;
	//double elapsed = 0.0;
	// while (elapsed < delta_time)
	// {
	// 	double t = min(per_tunneling, delta_time - elapsed);
	// 	simulation(t);
	// 	elapsed += t;
	// }

	int steps = static_cast<int>(std::ceil(delta_time / per_tunneling));
	double actual_step = delta_time / steps;
	for (int i = 0; i < steps; ++i)
		Simulation(actual_step);
}

void SPhysics::Simulation(const double delta_time)//注意tunneling，分批tick
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
		//静态物体不参与物理修正
		if (A->type == PhysicsType::Static)
			continue;
		for (auto& col : col_objs)
		{
			switch (col->type)
			{
				case PhysicsType::Static:	//和静态物体碰撞
					OnStaticBodyCollision(A,col);
					break;
				case PhysicsType::Movable:	//和可移动物体碰撞
					OnMovableBodyCollision(A,col);
					break;
				default: ;
			}
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
		A->CallDispatcher("OnCollision",col_objs);
	}
	TEndF("DispatcherOnCollision");
	TEnd;

	//鼠标点击事件
}

void SPhysics::HandlePhysics(const double delta_time, SPhysicsBaseUtility* unit) const
{
	//先记录模拟前的位置
	unit->after_location = unit->collision_owner->GetComponentWorldLocation();
	//重力势能
	Vec2<float> gravity_force = world_physics.GravityDir * WorldGravityForce * unit->mass;


	auto acceleration = gravity_force;

	unit->velocity += acceleration * delta_time;

	//处理冲量 可能来自 其他物体 或 主观
	unit->velocity += unit->added_force;	unit->added_force.Reset();	//消费
	//STOP_IF(unit->velocity.Length() < 1)
	//作用

	unit->after_location += (unit->velocity * delta_time).Cast<float>();

	//unit->velocity *= unit->force_attenuation;
}

void SPhysics::Synchronization() const
{
	TStart;
	for (auto& unit : units)
	{
		if (unit->type == PhysicsType::Static)
			continue;
		unit->dispatcher_system.CallDispatcher("OnSynchronization");
	}
	TEnd;
}


void SPhysics::OnRigidCollision(SPhysicsBaseUtility *A, SPhysicsBaseUtility *B)
{
//两个物体互相施加反作用力，
	//std::cout << "碰撞了" << std::endl;
}


void SPhysics::OnStaticBodyCollision(SPhysicsBaseUtility* A, const SPhysicsBaseUtility* B)
{
	auto ARect = A->GetCollisionBox();
	auto col_rect = B->GetCollisionBox();
	switch ( ARect.CollisionDir(col_rect))
	{
		case TOP:
			if (A->velocity.y < 0.f)
			{
				A->after_location.y = col_rect.y + col_rect.h;
				A->velocity.y = 0.f;
			}
			// A->added_force.y -= A->velocity.y;
			//std::cout << "Top碰撞" << std::endl;
			break;
		case BOTTOM:
			if (A->velocity.y > 0.f)
			{
				A->after_location.y = col_rect.y - ARect.h;
				A->velocity.y = 0.f;
			}
			// A->added_force.y -= A->velocity.y;
			//std::cout << "Bottom碰撞" << std::endl;
			break;
		case LEFT:
			A->after_location.x = col_rect.x + col_rect.w;
			A->velocity.x = 0.f;
			// A->added_force.x -=A->velocity.x;
			//std::cout << "Left碰撞" << std::endl;
			break;
		case RIGHT:
			A->after_location.x = col_rect.x - ARect.w;
			A->velocity.x = 0.f;
		default: ;
	}
}

void SPhysics::OnMovableBodyCollision(SPhysicsBaseUtility* A,SPhysicsBaseUtility* B)
{
	//先修正位置
	//OnStaticBodyCollision(A,B);


	const auto ARect = A->GetCollisionBox();
	const auto BRect = B->GetCollisionBox();

	const auto APoint = ARect.Center();
	const auto BPoint = BRect.Center();

	const auto overlapX = ARect.OverlapX(BRect);
	const auto overlapY = ARect.OverlapY(BRect);

	Vec2<float> n;
	float d;
	if (overlapX < overlapY)
	{
		n = Vec2{sign(APoint.x - BPoint.x), 0.f};
		d = overlapX;
	}
	else
	{
		n = Vec2{0.f, sign(APoint.y - BPoint.y)};
		d = overlapY;
	}
	// const Vec2<float> n = (APoint - BPoint).Normalized();
	Vec2<float> penetration_vec = n * d;
	A->after_location += penetration_vec * (B->mass / (A->mass + B->mass));
	B->after_location -= penetration_vec * (A->mass / (A->mass + B->mass));
	//添加冲量
	Vec2<float> relative_vel = A->velocity - B->velocity;
	auto vel_along_normal = Math::Dot2(relative_vel, n);
	if (vel_along_normal > 0)
	{
		return;
	}


	float e = std::min(A->bounciness, B->bounciness);
	float j = -(1 + e) * vel_along_normal;
	j /= A->mass_inv + B->mass_inv;

	auto f = n * j;
	A->velocity += f * A->mass_inv;
	B->velocity -= f * B->mass_inv;
}