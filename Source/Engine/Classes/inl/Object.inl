#pragma once
#include "Classes/Object.hpp"
#include "Utilities/function_traits.hpp"


template<typename ... Args>
void Object::CallDispatcher(const std::string& event_name, Args... args)
{
	EventParams e;
	// 把参数打包到 any vector
	e.Load({ std::forward<Args>(args)... });
	dispatcher_system.CallDispatcher(event_name, e);
}

// ----------------------
// 通用版本：接受任意 lambda / 可调用对象
// target: 发布者（拥有 dispatcher 的对象）
// dispatcher_name: 发布者的分发器名
// func: 任意可调用对象（捕获 lambda/非捕获 lambda/函数指针等）
// ----------------------
template<typename Func>
void Object::ListenDispatcher_Lambda(Object* target, const std::string& dispatcher_name, Func&& func)
{
	// 用 dispatcher_name 作为 lambda 名称基底，避免使用未定义的 name 成员
	const std::string lambda_name = name + "_lambda_" + std::to_string(++lambda_id);

	using Traits = function_traits<std::decay_t<Func>>;
	using ArgsTuple = typename Traits::args_tuple;

	auto argTypes = Traits::GetArgTypes();
	auto dispatcher = target->dispatcher_system.GetDispatcherType(dispatcher_name);
	if (!dispatcher) {
		Log("没有事件 " + dispatcher_name);
		return;
	}
	if (!dispatcher->CheckType(argTypes)) {
		Log("事件类型不匹配");
		return;
	}

	// 将 func decay-copy（可复制捕获），便于后续在事件中安全保存
	// auto func_copy = std::decay_t<Func>(std::forward<Func>(func));
	//
	// // Event 期望的回调签名在你的代码里通常是 void(TEventParams)
	// //using EventFuncType = std::function<void(TEventParams)>;
	//
	// auto invoker = [func_copy = std::move(func_copy)](TEventParams e) mutable {
	// 	 // 如果 lambda/func 无参（ArgsTuple size == 0），直接调用
	// 	 if constexpr (std::tuple_size_v<ArgsTuple> == 0) {
	// 		func_copy();
	// 	 } else {
	// 		// 否则从 TEventParams 中解包并调用
 // 			InvokeCallableWithEventParams<decltype(func_copy), ArgsTuple>(func_copy, e);
	// 	}
	// };

	Event event(lambda_name, std::move(func));
	//event.Traits(argTypes);
	AddCustomEvent(std::move(event));

	// 让 target 知道这个 delegate（事件名）
	target->AcceptDelegate(this, dispatcher_name, lambda_name);
}

//管成员函数
template<typename Listener, typename Ret, typename... Args>
void Object::ListenDispatcher(Object* target, const std::string& dispatcher_name, Ret(Listener::*func)(Args...))
{
	static_assert(std::is_base_of_v<Object, Listener>, "Listener must inherit from Object");


	const std::string lambda_name = name + "lambda" + std::to_string(++lambda_id);
	using EventFuncType = std::function<void(TEventParams)>;

	std::vector<std::type_index> type = { std::type_index(typeid(Args))... };

	if (!target->dispatcher_system.GetDispatcherType(dispatcher_name)->CheckType(type))
	{
		LogWithLevel(Error,"无法将{" + name + "}的成员函数绑定到  " + dispatcher_name);
	}
	auto event = Event(lambda_name,[listener = this, func](TEventParams e) {
					listener->template CallWithEventParams<Listener, Ret, Args...>(func, e, std::index_sequence_for<Args...>{});
				});
	// auto event = Event(lambda_name, [listener = this, func](auto&&... args){
	// 	(listener->*func)(std::forward<decltype(args)>(args)...);
	// });
	AddCustomEvent(std::move(event));
	// 这里模拟 target 注册 lambda（直接调用 AddCustomEvent）
	target->AcceptDelegate(this, dispatcher_name, lambda_name);
}

template<typename T, typename Ret, typename... Args, size_t... I>
void Object::CallWithEventParams(Ret(T::*func)(Args...), TEventParams e, std::index_sequence<I...>)
{
	(static_cast<T*>(this)->*func)(e->Get_index<std::decay_t<Args>>(I).value_or(Args{})...);
}