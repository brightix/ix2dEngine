#include "../../../Classes/Component/ActorComponent/RootComponent.h"

RootComponent::RootComponent()
{
	SceneComponent::name = "Root";
}

RootComponent::RootComponent(const Transform& transform)
{
	this->transform = transform;
}

