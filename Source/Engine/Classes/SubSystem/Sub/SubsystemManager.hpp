#pragma once
#include <string>
#include <type_traits>
#include <unordered_map>

#include "Utilities/GCPtr.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"
#include "Classes/SubSystem/GarbageCollection.hpp"

template<typename T>
class SubsystemManager : public Object
{
	std::unordered_map<uint32_t,GCPtr<T>> subSystem;
public:
    SubsystemManager()
    {
	    name = NAME("SubsystemManager");
    }
    ~SubsystemManager()= default;

	template<typename U>
	void CreateSubSystem(const std::string& subSys_name)
	{
		static_assert(std::is_base_of_v<T, U>, "U 必须继承自指定基类");
		const uint32_t id = ix::Hash(subSys_name.c_str());
		if (!subSystem.contains(id))
		{
			subSystem[id] = GCPtr<U>(new U(),this);
		}
		else
		{
			Log("重复添加子系统");
		}
	}

	T* GetSubSystem(const std::string& subSys_name)
	{
		uint32_t id = ix::Hash(subSys_name.c_str());
		if (subSystem.contains(id))
		{
			return subSystem[id].Get();
		}
		return nullptr;
	}
	std::vector<GCPtr<T>> GetAllSubSystem()
	{
		std::vector<GCPtr<T>> ret;
		for (auto& it : subSystem)
		{
			ret.emplace_back(it.second);
		}
		return ret;
	}
};

