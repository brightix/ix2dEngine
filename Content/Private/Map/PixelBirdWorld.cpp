#include "Public/Map/PixelBirdWorld.hpp"

#include "Classes/Controller.hpp"
#include "Public/GroundManager.hpp"
#include "Classes/Pawn.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Public/Bird.hpp"
#include "Public/BirdWidget.hpp"

PixelBirdWorld::PixelBirdWorld()
{
    CNAME;
}

void PixelBirdWorld::StartSimulation()
{
	GameWorld::StartSimulation();
	AddToViewport(CreateWidget<BirdWidget>());
	ListenDispatcher(GetController()->GetControlledPawn(),"OnGameOver","GameOver");
	ListenDispatcher(GetController(),"Key_F","StartGame");
}

void PixelBirdWorld::RegisterEvents()
{
	GameWorld::RegisterEvents();
	AddCustomEvent(Event("GameOver",[this] {
		ground_manager->Stop();
		Cast<MovableComponent>(World()->GetController()->GetControlledPawn()->GetActorComponent("MovableComponent"))->SetActiveMove(false);
	}));
	AddCustomEvent(Event("StartGame",[this](bool pressed) {
		if (pressed)
		{
			ground_manager->Init();
			auto bird = Cast<Bird>(World()->GetController()->GetControlledPawn());
			bird->start_game = true;
			bird->SetActorLocation({300,300});
			Cast<MovableComponent>(World()->GetController()->GetControlledPawn()->GetActorComponent("MovableComponent"))->SetActiveMove(true);
		}
	}));
}


void PixelBirdWorld::ConstructWorld()
{
	GameWorld::ConstructWorld();
	ground_manager = SpawnActor<GroundManager>();
}

void PixelBirdWorld::NativeWorldTick(const double delta_time)
{
	GameWorld::NativeWorldTick(delta_time);
}


