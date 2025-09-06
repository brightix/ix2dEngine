#include "Classes/Pawn.hpp"
Pawn::Pawn()
{

}

Pawn::~Pawn()
= default;

void Pawn::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);

}

void Pawn::HandleInput() {}
