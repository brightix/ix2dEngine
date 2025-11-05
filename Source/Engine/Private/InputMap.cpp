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

	auto mapping_file = OpenInputFileSafety("Source/Engine/Config/EnhancedInputMap.json");
	json mapping;
	mapping_file >> mapping;
	mapping_file.close();

	auto Normal_map = mapping["InputAction"];
	for (auto& key_info : Normal_map)
	{
		string name = key_info["KeyName"].get<string>();
		Normal[key_info["ScanCode"].get<SDL_Scancode>()] = InputAction(name,key_info["ScanCode"].get<SDL_Scancode>(),key_info["InputMode"]);
	}
	
	//注册增强输入事件
	for (auto key_info : mapping["EnhancedInputAction"])
	{
		string name = key_info["KeyName"].get<string>();
		Enhanced[key_info["ScanCode"].get<SDL_Scancode>()] = InputAction(name,key_info["ScanCode"].get<SDL_Scancode>(),key_info["InputMode"]);
	}

	for (auto button_info : mapping["MouseAction"])
	{
		string name = button_info["ButtonName"].get<string>();
		Mouse[button_info["ButtonId"].get<MouseCode>()] = MouseAction(name,button_info["ButtonId"].get<MouseCode>(),button_info["InputMode"]);
	}

}
