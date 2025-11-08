#include "Classes/GameModeBase.hpp"

#include "Classes/Character.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Public/TestPawn.hpp"

GameModeBase::~GameModeBase() {}

void GameModeBase::Tick(double deltaTime)
{
    //Actor::Tick(deltaTime);
//控制器
    //controller->Tick(deltaTime);

//UI

	//std::cout << controller->GetControlledPawn()->GetWorldLocation() << std::endl;
}

void GameModeBase::Construct()
{
	Actor::Construct();
	SetHiddenInGame(true);
}

void GameModeBase::EventBegin()
{
	Actor::EventBegin();
	controller = CreateController();
	//controller = SpawnActorFromSelf(new Controller());
	auto_generate_pawn = SpawnActor<Character>(Transform({300,300}));

	controller->Possess(auto_generate_pawn.Get());
}

Controller *GameModeBase::CreateController() const
{
	return game_world->AddController();
}

// TODO AIController
