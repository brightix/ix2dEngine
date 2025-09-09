#include "Utilities/Event.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"

Event::Event(const std::string& event_name, std::function<void(std::optional<EventParams>)> event_func)
{

}

void Event::Execute(std::optional<EventParams>&& event_params)
{
    if (event_func)
    {
        event_func(std::move(event_params));
    }
    else
    {
        Log("调用了空事件，event_func为nullptr");
    }
}
