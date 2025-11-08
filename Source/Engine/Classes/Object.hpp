#pragma once
#include <string>

#include "Core/EventDispatcherSystem.hpp"
 #include "Core/EventSystem.hpp"
 #include "Core/GCObject.hpp"
 #include "Utilities/FuncLib/SystemLib.hpp"

inline size_t lambda_id = 0;

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
	//注册自定义事件
	virtual void RegisterEvents(){}
	//注册事件分发器
	virtual void RegisterDispatchers(){}

	//template<typename Func>
	//void AddCustomEvent(const std::string& event_name, Func&& event_method);

	//void AddCustomEvent(const std::string& event_name, EventMethod&& event_method);


//事件处理函数
	void AddCustomEvent(Event&& event_method);

	void AddCustomEvent(const std::string& event_name, Event&& event);

	void CallEvent(const std::string& event_name, const TEventParams& params = std::nullopt);


	//添加事件分发器安全版本需要验证参数包类型
	//void DelegateEventDispatcher(Object *target, const std::string &event_name, Event event);


	//添加分发器
	void AddDispatcher(const std::string& event_name, const std::vector<std::type_index>& event_type = {});
	// 接收目标的委托事件名
	void AcceptDelegate(Object* delegate_obj, const std::string& dispatcher_name, const std::string& event_name);
	//解绑该对象的所有同名委托
	void RemoveDelegate(Object* dispatcher_owner, const std::string& dispatcher_name, const std::string& event_name);
	//调用分发器
	void CallDispatcher(const std::string& dispatcher_name, TEventParams event_params = std::nullopt);
	/**
	 * 监听目标对象事件
	 * @param target 监听对象
	 * @param dispatcher_name
	 * @param event_name 事件名
	 */
	void ListenDispatcher(Object* target, const std::string& dispatcher_name, const std::string& event_name);

	void IgnoreDispatcher(Object* target, const std::string& dispatcher_name);

	void IgnoreDispatcherEvent(Object* target, const std::string& dispatcher_name, const std::string& event_name);


	template<typename...Args>
	void CallDispatcher(const std::string& event_name, Args...args);

	template<class Func>
	void ListenDispatcher_Lambda(Object* target, const std::string& dispatcher_name, Func&& func);

	template<typename Listener, typename Ret, typename... Args>
	void ListenDispatcher(Object* target, const std::string& dispatcher_name, Ret(Listener::*func)(Args...));

	template<typename T, typename Ret, typename... Args, size_t... I>
	void CallWithEventParams(Ret(T::*func)(Args...), TEventParams e, std::index_sequence<I...>);

		//事件工具

		const std::string&& ValidEvent(const std::string&& event_name) const;



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

#include "Classes/inl/Object.inl"