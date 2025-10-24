#pragma once
#include "Controller.hpp"
#include "Classes/Actor.hpp"

class GameModeBase : public Actor
{
    GCPtr<Controller> controller;
    GCPtr<Pawn> auto_generate_pawn;

public:
    GameModeBase();
    ~GameModeBase() override;
    void Tick(double deltaTime) override;
	void Construct() override;
	void EventBegin() override;

    GCPtr<Controller> CreateController();
};

