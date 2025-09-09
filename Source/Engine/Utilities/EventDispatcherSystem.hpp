#pragma once
#include "Event.hpp"
#include "Types/EventParam.hpp"

using EventMethod = std::function<void(std::optional<EventParams>)>;

class EventDispatcherSystem
{
std::unordered_map<std::string,Event> dispatchers;
public:
    EventDispatcherSystem();
    ~EventDispatcherSystem()= default;
    void AddEventDispatcher(std::string event_name,EventMethod event_method);
};

