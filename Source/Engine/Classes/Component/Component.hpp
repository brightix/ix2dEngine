#pragma once
#include "Classes/Object.hpp"

class Actor;

class Component : public Object
{
protected:
	Actor* owner;
public:
	Component();
	Component(Actor* owner);
	void SetOwner(Actor* actor);
	Actor* GetOwner() const;
};
