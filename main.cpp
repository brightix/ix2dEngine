#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Classes/Actor.h"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"
#include "System/GameEngine.hpp"
#include "Classes/Controller.hpp"
#include "Content/Public/TestActor.h"
#include <fcntl.h>
using namespace std;
int main()
{
#ifdef _WIN32
    // ���ÿ���̨Ϊ UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    auto game_engine = GameEngine::Instance();
	//��Ϸʵ��
	//
    GameEngine::Instance().Construct();
    GameEngine::Instance().Tick();


    return 0;
}



