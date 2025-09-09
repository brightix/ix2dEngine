#include "Classes/InputMap.hpp"

#include "Utilities/json.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
using json = nlohmann::json;
using namespace std;
InputMap::InputMap() 
{
	//解析本地输入映射表
	auto file = OpenInputFileSafety("Source/Engine/Config/ControllerConfig.json");
	json controller_map;
	file >> controller_map;
	file.close();
	for (auto x : controller_map["InputAction"])
	{
		string name = x["KeyName"].get<string>();
		key_map[x["ScanCode"].get<SDL_Scancode>()] = InputAction(name,x["ScanCode"].get<SDL_Scancode>(),x["InputMode"]);
	}
}
