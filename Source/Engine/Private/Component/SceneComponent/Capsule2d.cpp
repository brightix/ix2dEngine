#include "Classes/Component/SenceComponent/Capsule2d.hpp"

Capsule2d::Capsule2d()
{
    CNAME;
}

void Capsule2d::Construct()
{
    SceneComponent::Construct();
    physics_body = NewObject<SPhysicsBaseUtility>(this);

}

void Capsule2d::ComponentEventBegin()
{
    SceneComponent::ComponentEventBegin();

    ListenDispatcher(physics_body.Get(),"OnSynchronization",Event([this](TEventParams) {
        SetComponentWorldLocation(physics_body->after_location);
    }));
}


void Capsule2d::NativeSetActiveCollision(const bool is_active)
{
    SceneComponent::NativeSetActiveCollision(is_active);
    if (physics_body)
    {
        physics_body->SetPhysicsType(PhysicsType::Movable);
    }
}


