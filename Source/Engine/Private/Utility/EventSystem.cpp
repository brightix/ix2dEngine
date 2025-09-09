#include "Utilities/EventSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"

EventSystem::EventSystem() {}
void EventSystem::AddEvent(const std::string& event_name,EventMethod e)
{
    events.try_emplace(event_name,e);
}

void EventSystem::CallEvent(const std::string& event_name, std::optional<EventParams> params)
{
    if (events.contains(event_name))
    {
        events[event_name](params);
    }
    else
    {
        Log("执行了不存在的事件，事件名：" + event_name);
    }
}



