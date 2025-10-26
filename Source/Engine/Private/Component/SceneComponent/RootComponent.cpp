#include "Classes/Component/SenceComponent/RootComponent.h"

RootComponent::RootComponent() : RootComponent(Transform{}) {}

RootComponent::RootComponent(const Transform& transform)
{
	NAME;
	this->transform = transform;
}

