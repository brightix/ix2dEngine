#include <iostream>
#include <GameInstance.hpp>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Actor.h"
#include <FuncLib/ixStaticFuncLib.hpp>
#include "Content/ixLib/TestActor.h"
using namespace std;
int main()
{
    Actor a(Transform(Vec2d<double>(50.0,50.0),{}));
    GameInstance::Instance().AddActor(make_shared<TestActor>(1));
    GameInstance::Instance().AddActor(make_shared<TestActor>(5));

    //游戏实例
    GameInstance::Instance().Ready();
    GameInstance::Instance().Tick();


    return 0;
}



