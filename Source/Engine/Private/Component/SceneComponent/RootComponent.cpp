#include "Classes/Component/SenceComponent/RootComponent.h"

RootComponent::RootComponent() : RootComponent(Transform{}) {}

RootComponent::RootComponent(const Transform& transform)
{
	CNAME;
	this->world_transform = transform;
}

