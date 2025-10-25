#include "Classes/GameModeBase.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Public/TestPawn.hpp"

GameModeBase::GameModeBase()
{}

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

}

void GameModeBase::EventBegin()
{
	Actor::EventBegin();
	controller = CreateController();
	//controller = SpawnActorFromSelf(new Controller());
	auto_generate_pawn = SpawnActor(new TestPawn());
	//auto_generate_pawn->EventBegin();

	controller->Control(auto_generate_pawn);
}

GCPtr<Controller> GameModeBase::CreateController()
{
	return game_world->AddController(new Controller());
}

// TODO AIController
