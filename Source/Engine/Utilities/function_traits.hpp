#pragma once
#include <tuple>
#include "Utilities/FuncLib/GlobalMacros.hpp"
#include "Types/EventParams.hpp"
// ========================
// 基础模板（函数签名）
// ========================

template<typename T>
struct function_traits;

// 普通函数
template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
	using return_type = Ret;
	using args_tuple = std::tuple<Args...>;
	static constexpr size_t arity = sizeof...(Args);

	static std::vector<std::type_index> GetArgTypes()
	{
		return { std::type_index(typeid(Args))... };
	}
};

// 函数指针
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

// std::function
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)> {};

// 非 const 成员函数
template<typename ClassType, typename Ret, typename... Args>
struct function_traits<Ret(ClassType::*)(Args...)> : function_traits<Ret(Args...)> {};

// const 成员函数
template<typename ClassType, typename Ret, typename... Args>
struct function_traits<Ret(ClassType::*)(Args...) const> : function_traits<Ret(Args...)> {};

// lambda（重载 operator()）
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};

template<typename Callable, typename Tuple, size_t... I>
static void InvokeCallableWithEventParamsImpl(Callable& callable, TEventParams e, std::index_sequence<I...>)
{
	callable(
		e->template Get_index<std::decay_t<std::tuple_element_t<I, Tuple>>>(I).value_or(std::tuple_element_t<I, Tuple>{})...
	);
}
template<typename Callable, typename Tuple, size_t... I>
static void InvokeCallableWithEventParamsImpl_Debug(Callable& callable, TEventParams e, std::index_sequence<I...>)
{
	// 使用 fold expression 展开参数
	// 先定义一个lambda，专门处理每个参数

	auto extractArg = [&](auto index_const) {
		constexpr size_t index = decltype(index_const)::value;
		using ArgType = std::decay_t<std::tuple_element_t<index, Tuple>>;

		auto valOpt = e->template Get_index<ArgType>(index);

		if (valOpt.has_value()) {
			LogWithLevel(Tip, std::format("[Invoke] 参数索引 {} 类型 {} 有值", index, typeid(ArgType).name()));
			return valOpt.value();
		}
		else {
			LogWithLevel(Warning, std::format("[Invoke] 参数索引 {} 类型 {} 无值，使用默认构造", index, typeid(ArgType).name()));
			return ArgType{};
		}
	};

	// 用 fold expression 展开参数提取过程
	callable(extractArg(std::integral_constant<size_t, I>{})...);
}

template<typename Callable, typename Tuple>
static void InvokeCallableWithEventParams(Callable& callable,TEventParams e)
{
	constexpr size_t N = std::tuple_size_v<Tuple>;
	InvokeCallableWithEventParamsImpl<Callable, Tuple>(callable, e, std::make_index_sequence<N>{});
}
