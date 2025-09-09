#pragma once
#include <string>

#include "Utilities/EventDelegateSystem.hpp"
#include "Utilities/EventDispatcherSystem.hpp"
#include "Utilities/EventSystem.hpp"
#include "Window/GCObject.hpp"
#include "Utilities/GCPtr.hpp"
class Object : public GCObject
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

	//生命周期
	//挂载的子object
	std::vector<Object> inner;
public:
    Object();
    Object* GetSelfPtr();


	//生命周期类

    void AddCustomEvent(const std::string& event_name,EventMethod event_method);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name,EventMethod event_method);

    ~Object() override = default;
};
