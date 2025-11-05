#include "Classes/Component/Component.hpp"

#include "Classes/Actor.hpp"

Component::Component() : Component(nullptr){ }
Component::Component(Actor* owner) : owned_actor(owner) { }



Actor* Component::GetOwner() const
{
	return owned_actor;
}

std::string Component::GetComponentName()
{
	return name;
}

bool Component::SetComponentName(const std::string& new_name)
{
	component_name = new_name;
	return true;
}
