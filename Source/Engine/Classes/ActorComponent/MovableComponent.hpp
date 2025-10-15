#pragma once

#include "ActorComponent.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"

class MovableComponent : public ActorComponent
{
    Vec2<float> player_input_Vec;
public:
    float base_move_speed;


    MovableComponent();
    MovableComponent(Actor* owner);
    void Construct() override;

    void ActorComponentTick(double deltaTime) override;

};