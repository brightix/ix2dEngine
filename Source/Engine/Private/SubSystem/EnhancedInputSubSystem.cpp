#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"

EnhancedInputSubSystem::EnhancedInputSubSystem() 
{
	std::cout<< "增强输入子系统初始化完毕" << std::endl;
}

void EnhancedInputSubSystem::AddInputEventBool(SDL_Scancode scancode, std::function<void(EnhancedInputParam<bool>)> func)
{
	if (!enhanced_input_event_bool.try_emplace(scancode,func).second)
	{
		Log("重复添加增强输入事件");
	}
}

void EnhancedInputSubSystem::ExecuteBool(EnhancedInputParam<bool> param)
{
	int t = param.input_action.scancode;
	auto it = enhanced_input_event_bool.find(param.input_action.scancode);
	if (it != enhanced_input_event_bool.end())
	{
		it->second(param);
	}
	else
	{
		Log("没有找到增强输入方法");
	}
}
