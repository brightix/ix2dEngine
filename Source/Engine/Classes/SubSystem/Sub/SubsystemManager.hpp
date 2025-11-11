#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>

#include "Utilities/GCPtr.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"

class SubSystemManager : public Object
{
	std::unordered_map<std::type_index,GCStrongPtr<SubsystemBase>> mounted_subsystem;
public:
    SubSystemManager()
    {
	    CNAME;
    }
    ~SubSystemManager() override
    {
    	std::cout << std::endl;
    }
	std::vector<SubsystemBase*> GetAllSubSystem()
	{
		std::vector<SubsystemBase*> ret;
		for (auto& sub : mounted_subsystem | std::views::values)
		{
			ret.emplace_back(sub.Get());
		}
		return ret;
	}

	void ForAllSubSystemInit()
	{
		for (auto& sub : mounted_subsystem | std::views::values)
		{
			sub->Init();
		}
	}

	void DeInitAllSubSystem()
	{
		for (auto& sub : mounted_subsystem | std::views::values)
		{
			sub->DeInit();
		}
    	mounted_subsystem.clear();
	}
	template<typename T>
	T* CreateSubsystem(Object* outer)
	{
		const std::type_index id = typeid(T);
		auto it = mounted_subsystem.find(id);
		if (it != mounted_subsystem.end())
		{
			Log("重复添加子系统");
			return nullptr;
		}
    	T* sub = NewObject<T>(outer);
		mounted_subsystem[id] = sub;
		return sub;
	}
	template<typename T>
	T* GetSubsystem()
	{
		const std::type_index id = typeid(T);
		auto it = mounted_subsystem.find(id);
		if (it == mounted_subsystem.end())
		{
			Log(std::string("没有实例化改子系统：") + typeid(T).name());
			return nullptr;
		}
		return static_cast<T*>(mounted_subsystem[id].Get());
	}
};


