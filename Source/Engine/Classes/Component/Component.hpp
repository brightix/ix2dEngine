#pragma once
#include "Classes/Object.hpp"

class Actor;

class Component : public Object
{
protected:
	std::string component_name;
	Actor* owner;
public:
	Component();
	Component(Actor* owner);
	void SetOwner(Actor* actor);
	Actor* GetOwner() const;
	std::string GetComponentName();
	void SetComponentName(const std::string& new_name);

	virtual void ComponentTick(double delta_time) {}
};
