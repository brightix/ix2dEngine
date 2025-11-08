#include "Utilities/Event.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"


bool Event::check(EventParams e) const
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

Event::Event() : Event("", [](TEventParams e){}) {}

// Event::Event(const std::string& event_name, const std::function<void(TEventParams)>& event_func)
// 	: event_name(event_name), event_func(std::function(event_func)), is_loop(false),
// 	  interval(0),
// 	  elapsed(0) {}
//
// Event::Event(const std::function<void(TEventParams)> &event_func) : Event("", event_func){}

// void Event::Execute() const
// {
// 	if (!event_func)  // move 给 lambda
// 		Log("调用了空事件，event_func为nullptr");
// 	if (!check({}))
// 	{
// 		std::cout << "参数无法对齐" << std::endl;
// 		return ;
// 	}
// 	event_func(std::nullopt);
// }
//
// void Event::Execute(const TEventParams& params) const
// {
// 	if (!event_func)  // move 给 lambda
// 		Log("调用了空事件，event_func为nullptr");
// 	if (!check(*params))
// 	{
// 		std::cout << "参数无法对齐" << std::endl;
// 		return ;
// 	}
// 	event_func(params);
// }
//
// void Event::Execute(std::optional<EventParams>&& params) const
// {
// 	if (!event_func)  // move 给 lambda
// 		Log("调用了空事件，event_func为nullptr");
// 	if (params && !check(*params))
// 	{
// 		std::cout << "参数无法对齐" << std::endl;
// 		return ;
// 	}
// 	event_func(std::move(params));
// }

void Event::Traits(const std::vector<std::type_index>& traits_type)
{
	event_type = std::vector<std::type_index>(traits_type);
}

void Event::operator()(TEventParams&& event_params) const
{
	event_func(std::forward<TEventParams>(event_params));
}
