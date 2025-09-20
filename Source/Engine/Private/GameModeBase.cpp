#include "Classes/GameModeBase.hpp"

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
    controller->Tick(deltaTime);

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
	controller = SpawnActorFromSelf(new Controller());
	controller->EventBegin();
	auto_generate_pawn = SpawnActorFromSelf(new Pawn());
	auto_generate_pawn->EventBegin();

	controller->Control(auto_generate_pawn.Get());
}
