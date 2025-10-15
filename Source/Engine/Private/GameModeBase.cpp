#include "Classes/GameModeBase.hpp"

#include "Classes/Core/GameWorld.hpp"

#include "Classes/Pawn.hpp"

GameModeBase::GameModeBase()
{
//关于游戏模式的初始化位置需要定义

}

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
	auto_generate_pawn = SpawnActor(new Pawn());
	//auto_generate_pawn->EventBegin();

	this->dispatcher_system.CallEvent("EventBegin");

	controller->Control(auto_generate_pawn);
}

GCPtr<Controller> GameModeBase::CreateController() const
{
	return game_world->AddController(new Controller());
}

// TODO AIController
