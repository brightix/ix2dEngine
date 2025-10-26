#pragma once
#include <string>
#include <unordered_map>

#include "Utilities/GCPtr.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

class SubSystemManager : public Object
{
	std::unordered_map<uint32_t,GCPtr<SubSystemBase>> mounted_subSystem;
public:
    SubSystemManager()
    {
	    NAME;
    }
    ~SubSystemManager()= default;

	template<typename T>
	T* CreateSubSystem(const std::string& subSys_name)
	{
		//static_assert(std::is_base_of_v<T, U>, "U 必须继承自指定基类");
		const uint32_t id = ix::Hash(subSys_name.c_str());
		if (!mounted_subSystem.contains(id))
		{
			GCPtr<T> sub = NewObject<T>(new T());
			dynamic_cast<SubSystemBase*>(sub.Get())->Init();
			mounted_subSystem.emplace(id,std::move(sub));
			return sub.Get();
		}
		Log("重复添加子系统");
		return nullptr;
	}
	template<typename T>
	GCWeakPtr<T> GetSubSystem(const std::string& subSys_name)
	{
		uint32_t id = ix::Hash(subSys_name.c_str());
		if (mounted_subSystem.contains(id))
		{
			return GCWeakPtr(mounted_subSystem[id].Get());
		}
		return {};
	}
	std::vector<GCWeakPtr<SubSystemBase>> GetAllSubSystem()
	{
		std::vector<GCWeakPtr<SubSystemBase>> ret;
		for (auto& sub : mounted_subSystem | std::views::values)
		{
			ret.emplace_back(GCWeakPtr(sub.Get()));
		}
		return ret;
	}

	void ForAllSubSystemInit()
	{
		for (auto& sub : mounted_subSystem | std::views::values)
		{
			sub->Init();
		}
	}

	void DeInitAllSubSystem()
	{
		for (auto& sub : mounted_subSystem | std::views::values)
		{
			sub->DeInit();
		}
	}
};


