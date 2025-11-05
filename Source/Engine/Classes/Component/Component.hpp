#pragma once
#include "Classes/Object.hpp"

class Actor;

class Component : public Object
{
protected:
	std::string component_name;
	Actor* owned_actor;
public:
	Component();

	explicit Component(Actor* owner);
	virtual void ComponentEventBegin(){}
	//void SetOwner(Actor* actor);
	[[nodiscard]] Actor* GetOwner() const;
	std::string GetComponentName();
	virtual bool SetComponentName(const std::string& new_name);

	virtual void ComponentTick(double delta_time) {}
};
