#pragma once
#include <string>
#include <unordered_map>

#include "Utilities/EventDispatcherSystem.hpp"


class EventSystem 
{
    std::unordered_map<std::string,EventMethod> events;
public:
    EventSystem();
    void AddEvent(const std::string& event_name, EventMethod);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    ~EventSystem()= default;
};

