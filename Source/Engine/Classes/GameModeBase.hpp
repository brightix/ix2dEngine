#pragma once
#include "Controller.hpp"
#include "Classes/Actor.h"

class GameModeBase : public Actor
{
    sptr<Controller> controller;
    sptr<Pawn> AutoGeneratePawn;

public:
    GameModeBase();
    ~GameModeBase() override;
    void Tick(double deltaTime) override;
};

