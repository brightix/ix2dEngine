#pragma once
#include <unordered_set>

#include "Utilities/Event.hpp"
#include "Types/EventParam.hpp"

class Object;
using EventMethod = std::function<void(std::optional<EventParams>)>;
//多播委托
class EventDispatcherSystem
{
	std::unordered_set<std::string> dispatchers;
	//分发器-> 事件对象-> 事件

	std::unordered_map<std::string,std::unordered_map<Object*,Event>> bound_dispatcher;

//	std::unordered_map<> ;
public:
    EventDispatcherSystem();
    ~EventDispatcherSystem()= default;


    //添加分发器
    void AddEventDispatcher(const std::string& name);

	//绑定事件到
    void BindEventTo(std::string event_name,Object* obj, Event event);

	void CallEvent(std::string event_name,std::optional<EventParams> event_params);
};

