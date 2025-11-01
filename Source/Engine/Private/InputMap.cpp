#include "Classes/InputMap.hpp"

#include "../Utilities/ThirdParty/json.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
using json = nlohmann::json;
using namespace std;
InputMap::InputMap() 
{
	CNAME;
}

void InputMap::Construct()
{
	Object::Construct();
	//解析本地输入映射表
	auto Enhanced_f = OpenInputFileSafety("Source/Engine/Config/EnhancedInputMap.json");
	json Enhanced_map;
	Enhanced_f >> Enhanced_map;
	Enhanced_f.close();


	auto Normal_f = OpenInputFileSafety("Source/Engine/Config/NormalInputMap.json");
	json Normal_map;
	Normal_f >> Normal_map;
	Normal_f.close();
	//注册普通事件
	for (auto x : Normal_map["InputAction"])
	{
		string name = x["KeyName"].get<string>();
		Normal[x["ScanCode"].get<SDL_Scancode>()] = InputAction(name,x["ScanCode"].get<SDL_Scancode>(),x["InputMode"]);
	}
	
	//注册增强输入事件
	for (auto x : Enhanced_map["EnhancedInputAction"])
	{
		string name = x["KeyName"].get<string>();
		Enhanced[x["ScanCode"].get<SDL_Scancode>()] = InputAction(name,x["ScanCode"].get<SDL_Scancode>(),x["InputMode"]);
	}


}
