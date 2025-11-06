#pragma once
#include "Classes/Pawn.hpp"
#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"


class TestPawn final : public Pawn
{
	GCPtr<StaticTextureComponent> default_texture;
public:
	using Pawn::Pawn;
    TestPawn();
	void Construct() override;
	void EventBegin() override;
};

