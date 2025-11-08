#pragma once
#include "Types/Dispatcher.hpp"
#include "Utilities/Event.hpp"
#include "Utilities/GCPtr.hpp"
#include "Utilities/GCWeakPtr.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"
class Object;
using EventMethod = std::function<void(TEventParams)>;

// struct DispatcherKey {
// 	std::string dispatcher;
// 	GCWeakPtr<Object> target;
// 	bool operator==(const DispatcherKey& o) const noexcept {
// 		return dispatcher == o.dispatcher && target == o.target;
// 	}
// };
//
// struct DispatcherKeyHash {
// 	size_t operator()(const DispatcherKey& k) const noexcept {
// 		return std::hash<std::string>{}(k.dispatcher)
// 			 ^ (std::hash<void*>{}(k.target.Get()) << 1);
// 	}
// };



//多播委托
class EventDispatcherSystem
{
	GCObject * outer;
	//分发器-> 事件对象-> 事件
	std::unordered_map<std::string,std::unordered_map<GCPtr<Object>,std::vector<Event>>> bound_dispatcher;

//					   分发器									委托者				   委托事件
	std::unordered_map<Dispatcher,      std::unordered_map<     GCWeakPtr<Object>,     Array<std::string>>> delegate;
//	std::unordered_map<std::string,     std::unordered_map<     GCWeakPtr<Object>,     std::unordered_set<std::string>>> delegate_set;
public:
    explicit EventDispatcherSystem(GCObject* outer_);
    ~EventDispatcherSystem()= default;

	const Dispatcher* GetDispatcherType(const std::string& dispatcher_name) const;

	//添加分发器
	void AddDispatcher(const std::string& event_name, const std::vector<std::type_index>& dispatcher_type = {});

	// 接收目标的委托事件名
	void AcceptDelegate(Object* target, std::string dispatcher_name, std::string event_name);
	//解绑该对象的所有同名委托
	void RemoveDelegate(Object* target, const std::string& dispatcher_name, const std::string& event_name);

	//调用分发器
	void CallDispatcher(const std::string& dispatcher_name, std::optional<EventParams>&& event_params = std::nullopt);

	void RemoveAllEventByDispatcher(Object* sender, const std::string& dispatcher_name);

	void RemoveAllEventByEventName(Object* sender, const std::string& dispatcher_name, const std::string& event_name);
};

