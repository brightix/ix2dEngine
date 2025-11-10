#pragma once
#include "Classes/Core/GameWorld.hpp"
#include "Public/GroundManager.hpp"
#include "Public/TestGround.hpp"


class PixelBirdWorld : public GameWorld
{
public:
    PixelBirdWorld();
	void StartSimulation() override;
	//void Construct() override;
	void RegisterEvents() override;
	void ConstructWorld() override;
	void NativeWorldTick(double delta_time) override;

    GCPtr<GroundManager> ground_manager;
};

