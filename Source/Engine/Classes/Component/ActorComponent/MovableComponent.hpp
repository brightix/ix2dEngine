#pragma once

#include "ActorComponent.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"

class Character;

class MovableComponent final : public ActorComponent
{
    Vec2<float> player_input_Vec;
    Character* c;
public:
    float base_move_speed;

    MovableComponent();
    void Construct() override;
    void Jump() const;
    void ActorComponentTick(double deltaTime) override;

};
