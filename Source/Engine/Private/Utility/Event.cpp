#include "Utilities/Event.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"


bool Event::check(const EventParams& e) const
{
	if (event_type.size() != e.types.size()) return false;
	for (int i = 0; i < event_type.size(); i++)
	{
		if (event_type[i] != e.types[i] || !IsBasicType(event_type[i]) && !IsBasicType(e.types[i]))
		{
			LogWithLevel(Error, "类型不匹配");
			return false;
		}
	}
	return true;
}

Event::Event() : Event("", [](TEventParams){}) {}

void Event::Traits(const std::vector<std::type_index>& traits_type)
{
	event_type = std::vector<std::type_index>(traits_type);
}

void Event::operator()(TEventParams& event_params) const
{
	if (event_params)
	{
		std::vector<std::type_index>& types = event_params->types;
		for (int i = 0; i < event_type.size(); i++)
		{
			auto ti1 = event_type[i];
			auto ti2 = types[i];
			if (ti1 == types[i])
			{
				continue;
			}
			if (!IsBasicType(event_type[i]) && !IsBasicType(types[i]))
			{

				continue;
			}
			LogWithLevel(Error, "类型不匹配");
		}
	}
	event_func(event_params);
}
