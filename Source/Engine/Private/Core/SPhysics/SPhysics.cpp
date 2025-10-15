#include "Classes/Core/SPhysics/SPhysics.hpp"

#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"
#include "Utilities/QuadTree.hpp"

SPhysics::SPhysics()
{
    collision_tree.SetCollisionStrategy([](SPhysicsBaseUtility *A, SPhysicsBaseUtility *B) {
        OnRigidCollision(A, B);
    });
}

void SPhysics::Register(SPhysicsBaseUtility* actor)
{
    actors.insert(actor);
}

void SPhysics::simulation(float delta_time)//注意tunneling
{
    collision_tree.clear();
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
    	if (1);
        auto* actor = *it;
        actor->HandleVelocity(delta_time);
        collision_tree.Insert(actor);
    }
    collision_tree.Query();
    // TODO 四叉树碰撞
}

void SPhysics::OnRigidCollision(SPhysicsBaseUtility *A, SPhysicsBaseUtility *B)
{
//两个物体互相施加反作用力，

}



