#pragma once
#include <string>

#include "Utilities/EventDelegateSystem.hpp"
#include "Core/EventDispatcherSystem.hpp"
#include "Core/EventSystem.hpp"
#include "../System/GCObject.hpp"
#include "Utilities/GCPtr.hpp"
class Object : public GCObject
{
public:
    std::string name;
protected:
    Object* Self;
    //事件分发器
    //委托
    EventDispatcherSystem dispatcher_system;

    //EventDelegateSystem delegate_system;
	//事件系统
    EventSystem event_system;

	//生命周期
	//挂载的子object
	//std::vector<Object> inner;
public:
    Object();
    Object* GetSelfPtr();

	virtual void Construct(){}
	//生命周期类

    void AddCustomEvent(const std::string& event_name, EventMethod&& event_method);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name, EventMethod&& event_method);
	template<typename T>
	GCPtr<T> ConstructObjectFromClass(T* object)
	{
		auto it = GCPtr<T>(object,this);
		static_cast<Object*>(it.Get())->Construct();
		return it;
	}
	void BindEvent(std::string dispatcher_name, EventMethod method);
    ~Object() override = default;
};
