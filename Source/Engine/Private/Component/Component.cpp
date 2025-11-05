#include "Classes/Component/Component.hpp"

#include "Classes/Actor.hpp"

Component::Component() : Component(nullptr){ }
Component::Component(Actor* owner) : owner(owner) { }



Actor* Component::GetOwner() const
{
	return owner;
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
