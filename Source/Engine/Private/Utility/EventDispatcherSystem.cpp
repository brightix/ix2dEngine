#include "Utilities/EventDispatcherSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
using namespace std;
EventDispatcherSystem::EventDispatcherSystem() {}
void EventDispatcherSystem::AddEventDispatcher(std::string event_name, function<void(optional<EventParams>)> event_method)
{
    if (!dispatchers.try_emplace(event_name,Event(event_name,event_method)).second)
    {
        LogWithLevel("添加["+ event_name +"]事件错误：已有相同事件写入事件系统",LogLevel::FatalError);
    }
}
