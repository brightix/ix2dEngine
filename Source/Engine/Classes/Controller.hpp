#pragma once
#include <iostream>

#include "Pawn.hpp"
#include "Window/GlobalMacros.hpp"

class Controller : public Actor
{
    sptr<Pawn> controlled_pawn;

public:
    Controller();
    ~Controller()= default;
    void Tick(double delta) override;
};

