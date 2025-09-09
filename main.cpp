#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Classes/Actor.h"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"
#include "Window/GameEngine.hpp"
#include "Classes/Controller.hpp"
#include "Content/Public/TestActor.h"
#include <fcntl.h>
using namespace std;
int main()
{
#ifdef _WIN32
    // 设置控制台为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    GameEngine::Instance();
    Actor a(Transform(Vec2d<double>(50.0,50.0),{}));
	GCPtr<Actor> c = make_GCPtr<Actor>(GameEngine::Instance().GetGameWorld().get());
	//游戏实例
    GameEngine::Instance().Ready();
    GameEngine::Instance().Tick();


    return 0;
}



