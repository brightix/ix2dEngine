#pragma once
#include "Classes/Actor.h"

class Pawn : public Actor
{
public:
    Pawn();
    ~Pawn() override;
    void Tick(double deltaTime) override;
    void HandleInput();
};

