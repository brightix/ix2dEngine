#pragma once
#include "Pawn.hpp"

class Capsule2d;
class StaticTextureComponent;

enum CharacterMoveStrategy
{
    God,
    Simulation
};

class Character : public Pawn
{
    GCPtr<StaticTextureComponent> sprite;
    using Pawn::Pawn;
    Capsule2d* capsule;
    CharacterMoveStrategy strategy;
public:
    void Construct() override;
    void EventBegin() override;

	void RegisterDispatchers() override;
	void RegisterEvents() override;
	void Possessed(Controller* possessed_controller) override;
	void UnPossessed(Controller* possessed_controller) override;

    SPhysicsBaseUtility* GetCharacterPhysicsBody() const;
    CharacterMoveStrategy GetCharacterMoveStrategy() const;
    void SetCharacterMoveStrategy(CharacterMoveStrategy new_strategy);


	int Test(bool b);
};
