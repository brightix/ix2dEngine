#pragma once
#include <functional>

#include "GCPtr.hpp"
#include "Types/EventParams.hpp"


struct Event
{
	//事件名
    std::string event_name;
	//事件方法
	std::function<void(std::optional<EventParams>)> event_func;

	//事件属性
	bool is_loop;
	double interval;
	double elapsed;

	Event();
	Event(const std::string& event_name, std::function<void(std::optional<EventParams>)> event_func);
	//Event(Event& other);
	void Execute();

	void Execute(const std::optional<EventParams>& params);

	void Execute(std::optional<EventParams>&& params);

	~Event()= default;
	void operator()(std::optional<EventParams>&& event_params = std::nullopt);
};

