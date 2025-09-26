#pragma once
#include "Event.hpp"
#include "Types/EventParam.hpp"

using EventMethod = std::function<void(std::optional<EventParams>)>;
//多播委托
class EventDispatcherSystem
{
std::unordered_map<std::string,Event> dispatchers;
std::unordered_map<std::string,std::vector<std::string>> name_to_delegate;
//	std::unordered_map<> ;
public:
    EventDispatcherSystem();
    ~EventDispatcherSystem()= default;
    void AddEventDispatcher(std::string event_name,EventMethod event_method);
	void BindEvent(Event e);
};

