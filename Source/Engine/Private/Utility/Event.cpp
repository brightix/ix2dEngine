#include "Utilities/Event.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"


Event::Event(const std::string& event_name, const std::function<void(std::optional<EventParams>)>& event_func)
	: event_name(event_name), event_func(std::function(event_func)), is_loop(false),
	  interval(0),
	  elapsed(0) {}
Event::Event() : Event("", [](std::optional<EventParams> e){}) {}

Event::Event(const std::function<void(std::optional<EventParams>)> &event_func) : Event("", event_func){}

void Event::Execute() {
	if (event_func)
		event_func(std::nullopt);
	else
		Log("调用了空事件，event_func为nullptr");
}

void Event::Execute(const std::optional<EventParams>& params) {
	if (event_func)
		event_func(params);  // 按引用调用，不拷贝
	else
		Log("调用了空事件，event_func为nullptr");
}

void Event::Execute(std::optional<EventParams>&& params)
{
	if (event_func)
		event_func(std::move(params));  // move 给 lambda
	else
		Log("调用了空事件，event_func为nullptr");
}
void Event::operator()(std::optional<EventParams>&& event_params)
{
	Execute(std::forward<std::optional<EventParams>>(event_params));
}
