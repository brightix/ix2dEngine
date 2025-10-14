#include <iostream>
#include <SDL3/SDL.h>
#include "Classes/Actor.hpp"
#include "Types/Vec.hpp"
#include "Source/Engine/Classes/Core/GameEngine.hpp"
#include <fcntl.h>

#include "Classes/Pawn.hpp"
using namespace std;
int main()
{
#ifdef _WIN32
    //UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif
    auto& game_engine = GameEngine::Instance();

    game_engine.Construct();

	game_engine.EventBegin();


    game_engine.Tick();


    return 0;
}



