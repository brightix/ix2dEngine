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
	void Construct() override;
	Actor *GetOwnerActor() const;

	virtual void ComponentEventBegin(){}
	//void SetOwner(Actor* actor);
	[[nodiscard]] Actor* GetOwner() const;
	void NativeSetOuter(GCObject* new_owner) override ;
	std::string GetComponentName();
	virtual bool SetComponentName(const std::string& new_name);

	virtual void ComponentTick(const double delta_time) {}
};
