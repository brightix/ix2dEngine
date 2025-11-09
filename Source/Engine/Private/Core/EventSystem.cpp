#include "Classes/Core/EventSystem.hpp"
using namespace std;
EventSystem::EventSystem() 
{
    
}



// void EventSystem::ModifyEvent(const std::string& event_name,
// 						   std::function<void(std::optional<EventParams>)>&& event_method)
// {
// 	events[event_name] = Event(event_name,forward<function<void(std::optional<EventParams>)>>(event_method));
// }


void EventSystem::CallEvent(const std::string& event_name,TEventParams params)
{
	if (!events.contains(event_name))
	{
		Log("执行了不存在的事件，事件名：" + event_name);
	}
	events[event_name](params);
}

std::optional<Event> EventSystem::GetEventByName(const std::string& event_name)
{
	auto it = events.find(event_name);
	if (it == events.end())
	{
		Log("没找到" + event_name +"事件");
		return std::nullopt;

	}
	return it->second;
}

void EventSystem::TickEvent(double delta_time)
{
	// for (auto& it : tick_events)
	// {
	// 	double interval_time = it.second.interval;
	//
	// 	it.second.elapsed += delta_time;
	// 	while (it.second.elapsed >= interval_time)
	// 	{
	// 		it.second();
	// 		it.second.elapsed-=interval_time;
	// 	}
	// 	it.second.;
	// }
}
