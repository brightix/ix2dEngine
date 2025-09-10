#pragma once
#include <functional>

#include "GCPtr.hpp"
#include "Types/EventParam.hpp"

enum class ParamType
{
	Bool,
	Int,
	Float,
	Double,
	String,
};

struct IEventParam : GCObject{ virtual ~IEventParam() = default; };

template<typename T>
struct EventParam : IEventParam
{
	T value;
	template<typename U>
	explicit EventParam(U&& v) : value(std::forward<U>(v))
	{
	}
};

struct EventParams : GCObject
{
	//参数包
	std::unordered_map<std::string,GCPtr<IEventParam>> values;

	template<typename T>
	std::optional<T> Get(const std::string& val_name)
	{
		auto it = values.find(val_name);
		if (it != values.end())
		{
			return static_cast<EventParam<T>*>(it->second.Get())->value;
		}
		return std::nullopt;
	}
	//添加变量
	template<typename U>
	void Add(const std::string& val_name, U&& v)
	{
		using W = std::decay_t<U>;
		values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
	}


};

struct Event
{
	//事件名
    std::string event_name;
	//事件方法
    std::function<void(std::optional<EventParams>)> event_func;

    explicit Event(const std::string& event_name, std::function<void(std::optional<EventParams>)> event_func);

    void Execute(std::optional<EventParams>&& event_params = std::nullopt);

    ~Event()= default;
};

