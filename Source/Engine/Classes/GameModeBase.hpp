#pragma once
#include "Controller.hpp"
#include "Classes/Actor.h"

class GameModeBase : public Actor
{
    GCPtr<Controller> controller;
    GCPtr<Pawn> AutoGeneratePawn;

public:
    GameModeBase();
    ~GameModeBase() override;
    void Tick(double deltaTime) override;
};

