#pragma once
#include <string>

#include "Core/EventDispatcherSystem.hpp"
#include "Core/EventSystem.hpp"
#include "Core/GCObject.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

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

	virtual void Construct();
	//生命周期类
	virtual void RegisterEvents(){}



    void AddCustomEvent(const std::string& event_name, EventMethod&& event_method);
    void CallEvent(const std::string& event_name, const std::optional<EventParams> &params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name, EventMethod&& event_method);
	void BindEventToDispatcher(Object *target, const std::string &event_name, Event event);

    /**
     * 监听目标对象事件
     * @param target 监听对象
     * @param event_name 事件名
     * @param event 事件
     */
    void ListenDispatcher(Object* target, const std::string& event_name, Event event);


 //    template<typename T>
	// GCPtr<T> NewObject(T* object)
	// {
	// 	static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
	// 	auto it = GCPtr<T>(object,this);
	// 	it->Construct();
	// 	return it;
	// }

	template<typename T>
	T* NewObject(T* ptr, Object *outer = nullptr)
    {
    	static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		auto object = IsDerived<Object>(ptr);
    	if (!object)
    	{
    		return nullptr;
    	}
    	object->Construct();
		GCAllObjects.push_back(object);
		if (outer)
		{
    		object->outer = outer;
			GCLink(outer,object);
		}
#if DEBUG
		if (!outer)
		{
			LogWithLevel(Warning,"构造了没有outer的对象");
		}
#endif
    	return ptr;
    }
	template<typename T,typename ...Args>
	T* NewObject(Object *outer = nullptr, Args... args)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		T* object = new T(std::forward<Args>(args)...);
		object->outer = outer;
		object->Construct();

		GCAllObjects.push_back(object);
		if (outer)
		{
			object->NativeSetOuter(outer);
			GCLink(outer,object);
		}
#if DEBUG
		if (!outer)
		{
			LogWithLevel(Warning,"构造了没有outer的对象");
		}
#endif
		return object;
	}

	template<typename T>
	GCPtr<T> NewObjectNoOuter(T* object)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		auto it = GCPtr<T>(object,nullptr);
		it->Construct();
		return it;
	}

    ~Object() override = default;
};
