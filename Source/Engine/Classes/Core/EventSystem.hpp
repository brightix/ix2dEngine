#pragma once
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../Utilities/Event.hpp"


class Object;
using EventMethod = std::function<void(std::optional<EventParams>)>;

class EventSystem
{
	Object* owner;
    std::unordered_map<std::string,Event> events;
public:
    EventSystem();
//尝试添加事件
	template<typename T>
    bool AddEvent(T&& event)
    {
		static_assert(std::is_same_v<std::decay_t<T>,Event>,"AddEvent 只允许传 Event 类型");

    	auto& name = event.event_name;
    	auto it = events.find(name);
    	if (it == events.end())
    	{
    		events[name] = event;
    		return true;
    	}
    	return false;
    }
//改写事件
    //void ModifyEvent(std::function<void(std::optional<EventParams>)>&& event_method);
//调用事件
    void CallEvent(const std::string& event_name,std::optional<EventParams> params = std::nullopt);

	void SetTimerEvent();
//带时间的轮训事件
	void TickEvent(double delta_time);
    ~EventSystem()= default;
};

