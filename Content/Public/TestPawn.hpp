#pragma once
#include "Classes/Pawn.hpp"
#include "Classes/Component/SenceComponent/CollisionBox.hpp"


class TestPawn : public Pawn
{
	GCWeakPtr<CollisionBox> collision_box;
public:
	using Pawn::Pawn;
    TestPawn();
	void Construct() override;

};

