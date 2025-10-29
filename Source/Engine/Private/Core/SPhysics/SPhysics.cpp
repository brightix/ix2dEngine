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

void SPhysics::simulation(double delta_time)//注意tunneling，分批tick
{
    collision_tree.Clear();
    for (auto& unit : units)
    {
    	unit->HandleVelocity(delta_time);
    	collision_tree.Insert(unit);
    }
    collision_tree.Query();


	//鼠标点击事件

    // TODO 四叉树碰撞
}

void SPhysics::HandlePhysics(double delta_time, SPhysicsBaseUtility* unit)
{
	unit->velocity.y += world_physics.GravityForce * delta_time * unit->quality;

	//处理冲量 可能来自 其他物体 或 主观
	unit->velocity += unit->added_force;
	unit->added_force.Reset();


}


void SPhysics::OnRigidCollision(SPhysicsBaseUtility *A, SPhysicsBaseUtility *B)
{
//两个物体互相施加反作用力，
	//std::cout << "碰撞了" << std::endl;
}




