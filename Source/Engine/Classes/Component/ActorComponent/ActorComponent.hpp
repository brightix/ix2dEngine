#pragma once
#include "Classes/Object.hpp"
#include "Classes/Component/Component.hpp"
class Actor;

class ActorComponent : public Component
{
protected:
    Actor* owner;
public:
    ActorComponent();
    ActorComponent(Actor* owner);
    virtual void EventBegin(){}
    void Construct() override;
    void SetOwner(Actor* owner_actor);
    virtual void ActorComponentTick(double delta_time){}
};
