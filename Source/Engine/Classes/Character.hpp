#pragma once
#include "Pawn.hpp"

class Capsule2d;
class StaticTextureComponent;

enum CharacterMoveStrategy : int
{
    God,
    Simulation,
	MoveStrategySize
};

class Character : public Pawn
{
    using Pawn::Pawn;
protected:
    GCPtr<StaticTextureComponent> sprite;
    Capsule2d* capsule;
    int move_strategy;
public:
    void Construct() override;
    void EventBegin() override;

	void RegisterDispatchers() override;
	void RegisterEvents() override;
	void Possessed(Controller* possessed_controller) override;
	void UnPossessed(Controller* possessed_controller) override;

    [[nodiscard]] SPhysicsBaseUtility* GetCharacterPhysicsBody() const;
    [[nodiscard]] CharacterMoveStrategy GetCharacterMoveStrategy() const;
    void SetCharacterMoveStrategy(CharacterMoveStrategy new_strategy);

	void SetCharacterOpenPhysics(PhysicsType new_physics) const;

	int Test(bool b);
};
