#include "Classes/Component/Component.hpp"

#include "Classes/Actor.hpp"

Component::Component() : Component(nullptr){ }
Component::Component(Actor* owner) : owner(owner) { }

void Component::SetOwner(Actor* actor)
{
	owner = actor;
}

Actor* Component::GetOwner() const
{
	return owner;
}
