#pragma once
#include <functional>

#include "function_traits.hpp"
#include "GCPtr.hpp"
#include "Types/EventParams.hpp"


struct Event
{
private:
	std::vector<std::type_index> event_type; // 用 type_index 存类型信息
	bool check(const EventParams& e) const;
public:
	//事件名
    std::string event_name;
	//事件方法
	//std::shared_ptr<std::function<void(std::optional<EventParams>)>> event_func;
	std::function<void(TEventParams)> event_func;

	//事件属性
	bool is_loop;
	double interval;
	double elapsed;

	Event();
	//Event(const std::string &event_name, const std::function<void(TEventParams)> &event_func);
	template<typename Func>
	Event(const std::string& event_name, Func&& func)
		: event_name(event_name), is_loop(false), interval(0), elapsed(0)
	{
		using traits = function_traits<std::decay_t<Func>>;
		using ArgsTuple = typename traits::args_tuple;
		const auto func_copy = std::decay_t<Func>(std::forward<Func>(func));
		const std::vector<std::type_index> argTypes = traits::GetArgTypes();

		event_func = [func_copy = std::move(func_copy)](TEventParams e) mutable {
			if constexpr (std::tuple_size_v<ArgsTuple> == 0) {
				func_copy(); // 无参直接调用
			} else if constexpr (std::tuple_size_v<ArgsTuple> == 1 &&
								 std::is_same_v<std::tuple_element_t<0, ArgsTuple>, TEventParams>) {
				func_copy(e); // 单参数 TEventParams lambda，直接传入
								 } else {
								 	InvokeCallableWithEventParams<decltype(func_copy), ArgsTuple>(func_copy, e);
								 }
		};

		Traits(argTypes);
	}
	void Traits(const std::vector<std::type_index>& traits_type);

	void operator()(const std::optional<EventParams>& event_params) const;


	const std::vector<std::type_index>& GetType() const
	{
		return event_type;
	}
	~Event()= default;

	void operator()(TEventParams& event_params) const;
};

