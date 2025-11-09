#include "Classes/Component/SenceComponent/Capsule2d.hpp"

#include "Types/Enums/PivotDir.hpp"

Capsule2d::Capsule2d()
{
    CNAME;
}

void Capsule2d::Construct()
{
    SceneComponent::Construct();
    physics_body = NewObject<SPhysicsBaseUtility>(this);
	pivot = PivotDir::BOTTOM_CENTER;
}

void Capsule2d::RegisterEvents()
{
	SceneComponent::RegisterEvents();
	AddCustomEvent({"Synchronization",[this]() {
		SetComponentWorldLocation(physics_body->after_location);
	}});
}

void Capsule2d::RegisterDispatchers()
{
	SceneComponent::RegisterDispatchers();
	AddDispatcher("OnCollision",{TypeID(std::unordered_set<SPhysicsBaseUtility*>)});
}

void Capsule2d::ComponentEventBegin()
{
    SceneComponent::ComponentEventBegin();

    ListenDispatcher(physics_body.Get(),"OnSynchronization","Synchronization");
	ListenDispatcher(physics_body.Get(),"OnCollision",&Capsule2d::Collision);
}


void Capsule2d::NativeSetActiveCollision(const bool is_active)
{
    SceneComponent::NativeSetActiveCollision(is_active);
    if (physics_body)
    {
        physics_body->SetPhysicsType(PhysicsType::Movable);
    }
}

void Capsule2d::Collision(std::unordered_set<SPhysicsBaseUtility*> cols)
{
	CallDispatcher("OnCollision",cols);
}


