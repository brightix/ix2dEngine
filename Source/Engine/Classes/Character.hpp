#pragma once
#include "Pawn.hpp"

class StaticTexture;

class Character : public Pawn
{
    GCPtr<StaticTexture> sprite;
public:
    Character();
    void Construct() override;

};
