#include "Classes/Core/EventDispatcherSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
using namespace std;
EventDispatcherSystem::EventDispatcherSystem() {}
void EventDispatcherSystem::AddEventDispatcher(const std::string& event_name)
{
    // if (!dispatchers.try_emplace(event_name,Event(event_name,event_method)).second)
    // {
    //     LogWithLevel("添加["+ event_name +"]事件错误：已有相同事件写入事件系统",LogLevel::FatalError);
    // }
}

//在这个事件系统里绑定其他对象的事件
void EventDispatcherSystem::BindEventTo(std::string event_name, Object *obj, Event event)
{
    auto it = bound_dispatcher.find(event_name);
    if (it == bound_dispatcher.end())
    {
        bound_dispatcher[event_name][obj] = event;
    }
    else
    {
        Log("Bind to Not Created Event");
    }
}

void EventDispatcherSystem::CallEvent(std::string event_name, std::optional<EventParams> event_params)
{
    auto it = bound_dispatcher.find(event_name);
    if (it != bound_dispatcher.end())
    {
        for (auto& method : it->second)
        {
            method.second(std::move(event_params));
            Log("执行了  "+ event_name + "  的事件！");
        }
    }
    else
    {
        Log("没有名为  "+ event_name + "  的事件！");
    }
}

