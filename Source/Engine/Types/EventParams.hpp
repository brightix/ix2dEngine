#pragma once
#include <optional>
#include <unordered_map>
#include <any>

enum class ParamType
{
	Bool,
	Int,
	Float,
	Double,
	String,
};

struct IEventParam{};

template<typename T>
struct EventParam : IEventParam
{
	T value;
	template<typename U>
	explicit EventParam(U&& v) : value(std::forward<U>(v)){}
};

struct EventParams
{
	//参数包
	std::unordered_map<std::string,std::any> values;

	template<typename T>
	std::optional<T> Get(const std::string& val_name)
	{
		auto it = values.find(val_name);
		if (it != values.end())
		{
			if (auto ret = std::any_cast<T>(&it->second))
			{
				return *ret;
			}
		}
		return std::nullopt;
	}
	//添加变量
	template<typename U>
	void Add(const std::string& val_name, U&& v)
	{
		//using W = std::decay_t<U>;
		//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
		values[val_name] = std::forward<U>(v);
	}
	template<typename U>
	void Add(const std::string& val_name, U& v)
	{
		//using W = std::decay_t<U>;
		//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
		values[val_name] = v;
	}
};