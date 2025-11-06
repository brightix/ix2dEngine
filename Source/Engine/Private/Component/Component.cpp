#include "Classes/Component/Component.hpp"

#include "Classes/Actor.hpp"

Component::Component() : owned_actor(nullptr) {}

Actor* Component::GetOwnerActor() const
{
	return Cast<Actor>(outer);
}

Actor * Component::GetOwner() const
{
	return owned_actor;
}

void Component::NativeSetOuter(GCObject *new_owner)
{
	Object::NativeSetOuter(new_owner);
	owned_actor = Cast<Actor>(new_owner);
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
