#pragma once
#include <string>

#include "Core/EventDispatcherSystem.hpp"
#include "Core/EventSystem.hpp"
#include "Core/GCObject.hpp"

class Object : public GCObject
{
protected:
    //事件分发器
    //EventDelegateSystem delegate_system;

public:
	//委托
	EventDispatcherSystem dispatcher_system;
	//事件系统
	EventSystem event_system;
    Object();

	virtual void Construct(){}
	//生命周期类

    void AddCustomEvent(const std::string& event_name, EventMethod&& event_method);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name, EventMethod&& event_method);

	template<typename T>
	GCPtr<T> NewObject(T* object)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		auto it = GCPtr<T>(object,this);
		it->Construct();
		return it;
	}
    ~Object() override = default;
};
