#pragma once
#include <string>

#include "Utilities/EventDelegateSystem.hpp"
#include "Utilities/EventDispatcherSystem.hpp"
#include "Utilities/EventSystem.hpp"

class Object
{
public:
    std::string name;
private:
    Object* Self;
    //事件分发器
    EventDispatcherSystem dispatcher_system;
    //委托
    EventDelegateSystem delegate_system;
    EventSystem event_system;
public:
    Object();
    Object* GetSelfPtr();
    void AddCustomEvent(const std::string& event_name,EventMethod event_method);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name,EventMethod event_method);
    virtual ~Object()= default;
};
