#include "Classes/GameModeBase.hpp"
GameModeBase::GameModeBase()
{
//关于游戏模式的初始化位置需要定义

}

GameModeBase::~GameModeBase() {}

void GameModeBase::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);
//控制器
    controller->Tick(deltaTime);

//UI


}
