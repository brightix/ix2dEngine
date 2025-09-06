#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Classes/Actor.h"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"
#include "Window/GameInstance.hpp"
#include "Classes/Controller.hpp"
#include "Content/Public/TestActor.h"
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



