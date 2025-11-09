#pragma once
#include "Classes/Core/GameWorld.hpp"
#include "Public/TestGround.hpp"


class PixelBirdWorld : public GameWorld
{
public:
    PixelBirdWorld();
	//void Construct() override;
	void ConstructWorld() override;
	void NativeWorldTick(double delta_time) override;
};

