#pragma once
#include "Classes/Object.hpp"
class Actor;

class ActorComponent : public Object
{
protected:
    Actor* owner;
public:
    ActorComponent();
    ActorComponent(Actor* owner);
    void Construct() override;
    virtual void ActorComponentTick(double delta_time){}
};
