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
	//auto boundary = unit->collision_owner->GetComponentCollisionBoundary();
	//先记录模拟前的位置
	unit->after_location = unit->collision_owner->GetComponentWorldLocation();//{boundary.x,boundary.y};
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
	auto pivot = A->collision_owner->GetComponentPivot(); // (0~1)

	switch (ARect.CollisionDir(col_rect))
	{
		case TOP:
			// A 头部撞到 B 的底部
			if (A->velocity.y < 0.f)
			{
				// 把 A 放在 B 下方，考虑 pivot 偏移
				A->after_location.y = col_rect.y + col_rect.h + ARect.h * pivot.y;
				A->velocity.y = 0.f;
			}
			// std::cout << "Top碰撞" << std::endl;
			break;

		case BOTTOM:
			// A 底部撞到 B 顶部
			if (A->velocity.y > 0.f)
			{
				// 让 A 贴在 B 上方，考虑 pivot 偏移
				A->after_location.y = col_rect.y - ARect.h * (1.f - pivot.y);
				A->velocity.y = 0.f;
			}
			if (std::abs(A->velocity.x) > 0.001f)
			{
				float N = WorldGravityForce * A->mass;
				float friction_acc = A->friction * N * A->mass_inv * 3.0f; // 放大系数
				float deltaV = friction_acc * GetDeltaTime();

				A->velocity.x -= sign(A->velocity.x) * deltaV;

				// 加上轻微指数阻尼，让停止更自然
				A->velocity.x *= std::pow(1.f - 0.05f * A->friction, GetDeltaTime() * 60.f);

				if (std::abs(A->velocity.x) < deltaV)
					A->velocity.x = 0.f;
			}
			// std::cout << "Bottom碰撞" << std::endl;
			break;

		case LEFT:
			// A 左边撞到 B 右边
			// 让 A 贴在 B 的右侧，考虑 pivot 偏移
			A->after_location.x = col_rect.x + col_rect.w + ARect.w * pivot.x;
			A->velocity.x = 0.f;
			// std::cout << "Left碰撞" << std::endl;
			break;

		case RIGHT:
			// A 右边撞到 B 左边
			// 让 A 贴在 B 的左侧，考虑 pivot 偏移
			A->after_location.x = col_rect.x - ARect.w * (1.f - pivot.x);
			A->velocity.x = 0.f;
			// std::cout << "Right碰撞" << std::endl;
			break;

		default:
			break;
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
	//
	// // --- 添加摩擦 ---
	// Vec2<float> tangent = relative_vel - n * Math::Dot2(relative_vel, n);
	// if (tangent.LengthSquared() > 0.0001f)
	// 	tangent = tangent.Normalized();
	//
	// float mu = (A->friction + B->friction) * 0.5f; // 平均摩擦系数
	// float jt = -Math::Dot2(relative_vel, tangent);
	// jt /= A->mass_inv + B->mass_inv;
	//
	// // 限制最大摩擦力（库仑摩擦模型）
	// if (std::abs(jt) > j * mu)
	// 	jt = j * mu * (jt > 0 ? 1.f : -1.f);
	// std::cout << "j=" << j << " jt=" << jt << " mu=" << mu << std::endl;
	// Vec2<float> frictionImpulse = tangent * jt;
	// A->velocity += frictionImpulse * A->mass_inv;
	// B->velocity -= frictionImpulse * B->mass_inv;
	// --- 添加摩擦力（Force-based 模型）---
	double dt = GetDeltaTime();
	Vec2<float> tangent = relative_vel - n * Math::Dot2(relative_vel, n);
	float tangent_len2 = tangent.LengthSquared();
	if (tangent_len2 > 0.0001f)
		tangent = tangent.Normalized();
	else
		tangent = Vec2<float>(0.f, 0.f);

	// 平均摩擦系数
	float mu = (A->friction + B->friction) * 0.5f;

	// 法向冲量转为法向力（近似 N = j / dt）
	float normal_force = std::abs(j) / dt;

	// 库仑摩擦模型：Ff = μ * N
	const Vec2<float> friction_force = -tangent * mu * normal_force;

	// --- 应用到速度更新（F = ma -> Δv = F/m * dt）---
	A->velocity += friction_force * (A->mass_inv * dt);
	B->velocity -= friction_force * (B->mass_inv * dt);

	// 调试输出
	// std::cout << "Ff=(" << friction_force.x << "," << friction_force.y << ") "
	// 		  << "N=" << normal_force << " mu=" << mu << std::endl;
}