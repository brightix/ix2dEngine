#pragma once
#include "Classes/Object.hpp"
#include "Classes/Component/Component.hpp"
class Actor;

class ActorComponent : public Component
{
public:
	using Component::Component;
    void Construct() override;
    virtual void ActorComponentTick(double delta_time){}
	void SetOwnerActor(Actor* actor);
};
