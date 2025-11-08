#pragma once
#include <functional>

#include "function_traits.hpp"
#include "GCPtr.hpp"
#include "Types/EventParams.hpp"


struct Event
{
private:
	std::vector<std::type_index> event_type; // 用 type_index 存类型信息
	bool check(EventParams e) const;
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
	Event(const std::string &event_name, Func&& func) : event_name(event_name), is_loop(false), interval(0), elapsed(0)
	{
		using traits = function_traits<std::decay_t<Func> >;
		using ArgsTuple = typename traits::args_tuple;

		const std::vector<std::type_index> argTypes = traits::GetArgTypes();

		// 将 func decay-copy（可复制捕获），便于后续在事件中安全保存
		const auto func_copy = std::decay_t<Func>(std::forward<Func>(func));

		auto invoker = [func_copy = std::move(func_copy)](TEventParams e) mutable {
			// 如果 lambda/func 无参（ArgsTuple size == 0），直接调用
			if constexpr (std::tuple_size_v<ArgsTuple> == 0)
			{
				func_copy();
			}
			else
			{
				// 否则从 TEventParams 中解包并调用
				InvokeCallableWithEventParams<decltype(func_copy), ArgsTuple>(func_copy, e);
			}
		};
		event_func = std::move(invoker);
		Traits(argTypes);
	}

	//Event(const std::function<void(TEventParams)> &event_func);


	//Event(Event& other);
	// void Execute() const;
	// void Execute(const TEventParams& params) const;
	// void Execute(TEventParams&& params) const;

	void Traits(const std::vector<std::type_index>& traits_type);


	const std::vector<std::type_index>& GetType() const
	{
		return event_type;
	}
	~Event()= default;
	void operator()(TEventParams&& event_params = std::nullopt) const;

};

