#pragma once
#include <functional>

#include "GCPtr.hpp"
#include "Types/EventParams.hpp"


struct Event
{
	//事件名
    std::string event_name;
	//事件方法
	//std::shared_ptr<std::function<void(std::optional<EventParams>)>> event_func;
	std::function<void(std::optional<EventParams>)> event_func;
	//事件属性
	bool is_loop;
	double interval;
	double elapsed;

	// template<typename Func>
	// Event(Func&& func) : event_func(std::forward<Func>(func)), is_loop(false), interval(0), elapsed(0) { }

	Event(const std::string &event_name, const std::function<void(std::optional<EventParams>)> &event_func);

	Event();

	Event(const std::function<void(TEventParams)> &event_func);

	//Event(Event& other);
	void Execute();

	void Execute(const std::optional<EventParams>& params);

	void Execute(std::optional<EventParams>&& params);

	~Event()= default;
	void operator()(std::optional<EventParams>&& event_params = std::nullopt);
};

