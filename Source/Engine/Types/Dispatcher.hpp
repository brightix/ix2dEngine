#pragma once
#include <optional>
#include <string>
#include <vector>

#include "EventParams.hpp"
#include "Utilities/FuncLib/GlobalMacros.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

namespace std
{
	class type_index;
}

struct Dispatcher
{
private:
	TEventParams event_params;
	std::string dispatcher_name;
	std::vector<std::type_index> type;
public:
	Dispatcher(){}
	Dispatcher(const std::string& dispatcher_name, const std::vector<std::type_index>& types = {}) : dispatcher_name(dispatcher_name), type(types) {}
	bool CheckType(const std::vector<std::type_index>& other_type) const
	{
		if (type.size() != other_type.size()) return false;
		for (int i = 0; i < type.size(); i++)
		{
			// bool a = IsBasicType(type[i]);
			// bool b = IsBasicType(other_type[i]);
			// bool c = type[i] != other_type[i];
			// if (c || b && a)
			if (type[i] != other_type[i] || !IsBasicType(type[i]) && !IsBasicType(other_type[i]))
			{
				LogWithLevel(Error, "类型不匹配");
				return false;
			}
		}
		return true;
	}
	const std::vector<std::type_index>& GetTypes() const
	{
		return type;
	}
	bool operator==(const Dispatcher& other) const noexcept
	{
		return dispatcher_name == other.dispatcher_name;
	}
	const std::string& GetDispatcherName() const
	{
		return dispatcher_name;
	}
};

namespace std
{
	template<>
	struct hash<Dispatcher>
	{
		size_t operator()(const Dispatcher& d) const noexcept
		{
			return std::hash<std::string>()(d.GetDispatcherName());
		}
	};
}