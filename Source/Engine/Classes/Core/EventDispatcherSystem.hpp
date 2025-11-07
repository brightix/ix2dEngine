#pragma once
#include <unordered_set>
#include "Utilities/Event.hpp"
#include "Utilities/GCPtr.hpp"
#include "Utilities/GCWeakPtr.hpp"


class Object;
using EventMethod = std::function<void(std::optional<EventParams>)>;
//多播委托
class EventDispatcherSystem
{
	GCObject * outer;
	//分发器-> 事件对象-> 事件
	std::unordered_map<std::string,std::unordered_map<GCPtr<Object>,std::vector<Event>>> bound_dispatcher;

	//分发器
	std::unordered_map<std::string,std::unordered_map<GCWeakPtr<Object>,std::unordered_set<std::string>>> delegate;

	//	std::unordered_map<> ;
public:
    EventDispatcherSystem(GCObject* outer_);
    ~EventDispatcherSystem()= default;


    //添加分发器
    void AddEventDispatcher(const std::string& event_name);

	//在这个事件系统里绑定其他对象的事件
    void BindEventTo(Object *obj, const std::string& bounded_name,  Event event);
	std::optional<Event> GetEvent(Object *obj, const std::string& event_name);
	void CallDispatcher(const std::string& event_name,std::optional<EventParams> event_params = std::nullopt);




	void DelegateEvent(Object* obj, std::string dispatcher_name, std::string event_name);
	void CallDelegate(const std::string& dispatcher_name);
};

