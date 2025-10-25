#include <iostream>
#include "Source/Engine/Classes/Core/GameEngine.hpp"
#include <crtdbg.h>
using namespace std;
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#ifdef _WIN32
    //UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    auto& game_engine = GameEngine::Instance();

	game_engine.Construct();

	int a = 0;
	game_engine.EventBegin();

	game_engine.Tick();

    return 0;
}



