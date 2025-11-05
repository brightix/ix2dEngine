#include <iostream>
#include <windows.h>

#include "Classes/Core/GameEngine.hpp"
#include "Utilities/TracingUtility.hpp"

// #include "Utilities/FuncLib/GlobalMacros.hpp"

using namespace std;
int main()
{
// #if DEBUG
// 	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
// #endif
#ifdef _WIN32
    //UTF-8
	setlocale(LC_ALL, ".utf-8"); // 设置标准库调用系统 API 所用的编码，用于 fopen，ifstream 等函数
	SetConsoleOutputCP(CP_UTF8); // 设置控制台输出编码，或者写 system("chcp 65001")
	SetConsoleCP(CP_UTF8);
#endif
	TracingUtility& i = TracingUtility::Instance();
    auto& game_engine = GameEngine::Instance();

	game_engine.Construct();
	//
	game_engine.EventBegin();
	//
	game_engine.Tick();

    return 0;
}



