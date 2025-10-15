#include "../../../Classes/Component/ActorComponent/RootComponent.h"

RootComponent::RootComponent()
{

}

RootComponent::RootComponent(const Transform& transform)
{
	this->transform = transform;
}

PhysicsType RootComponent::GetPhysicsType()
{
	return PhysicsType::Static;
}
