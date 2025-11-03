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

	explicit Component(Actor* owner);
	virtual void ComponentEventBegin(){}
	void SetOwner(Actor* actor);
	Actor* GetOwner() const;
	std::string GetComponentName();
	//virtual bool SetName(const std::string& new_name);

	virtual void ComponentTick(double delta_time) {}
};
