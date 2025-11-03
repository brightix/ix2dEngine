#pragma once
#include <string>
#include <vector>

#include "Core/EventDispatcherSystem.hpp"
#include "Core/EventSystem.hpp"

inline static size_t glo_id = 0;  // C++17

template<typename T>
class GCPtr;

class Object
{
public:
	//GC
	bool bMarked = false;
	bool is_pending_kill = false;
	size_t id;
	std::vector<Object*> referenced;
	std::vector<Object*> referencing;

	//委托
	EventDispatcherSystem dispatcher_system;
	//事件系统
	EventSystem event_system;
	std::string class_name = "UnknownClass";
	std::string name;



	[[nodiscard]] bool IsActive() const;
	[[nodiscard]] std::string GetClassName() const;
	void SetName(const std::string& new_name);



    Object();

	virtual void Construct(){}
	//生命周期类

    void AddCustomEvent(const std::string& event_name, EventMethod&& event_method);
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);
    //添加事件分发器安全版本需要验证参数包类型
    void AddEventDispatcher(const std::string& event_name, EventMethod&& event_method);
	void BindEventToDispatcher(Object *target, const std::string &event_name, Event event);

    /**
     * 监听目标对象事件
     * @param target 监听对象
     * @param event_name 事件名
     * @param event 事件
     */
	static void ListenDispatcher(Object* target, const std::string& event_name, Event event);


    template<typename T>
	GCPtr<T> NewObject(T* object)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		auto it = GCPtr<T>(object,this);
		it->Construct();
		return it;
	}
	template<typename T>
	GCPtr<T> NewObjectNoOuter(T* object)
	{
		static_assert(std::is_base_of_v<Object, T>, "T must derive from Object");
		auto it = GCPtr<T>(object,nullptr);
		it->Construct();
		return it;
	}

    virtual ~Object();
	//GC安全 只创建实例，不执行construct和绑定事件
	template<typename T, typename ...Args>
	GCPtr<T> NewGCPtr(Args&&...args)
	{
		return GCPtr<T>(new T(std::forward<Args>(args)...), this);
	}
	template<typename T>
	GCPtr<T> NewGCPtr(T* p)
	{
		return GCPtr<T>(p, this);
	}

	void GCUnlink()
	{
		for (auto parent : referenced)
		{
			std::erase(parent->referencing,this);
		}
		for (auto child : referencing)
		{
			std::erase(child->referenced,this);
		}
	}
};
