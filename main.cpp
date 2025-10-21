#include <iostream>
#include "Source/Engine/Classes/Core/GameEngine.hpp"
using namespace std;
int main()
{
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



