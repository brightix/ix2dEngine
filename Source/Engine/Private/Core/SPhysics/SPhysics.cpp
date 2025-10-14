#include "Classes/Core/SPhysics/SPhysics.hpp"

#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

void SPhysics::Register(SPhysicsUtilityBase* actor)
{
    actors.insert(actor);
}

void SPhysics::simulation(float delta_time)
{
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        auto* actor = *it;
        actor->HandleVelocity(delta_time);
        // switch (actor->GetPhysicsType())
        // {
        //     case PhysicsType::Static:
        //
        //         break;
        //     case PhysicsType::RigidBody:
        //         break;
        //     case PhysicsType::Movable:
        //         actor->;
        //         break;
        //     default: break;
        // }
        // TODO 四叉树碰撞
    }
}

