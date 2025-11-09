#pragma once
#include <optional>
#include <unordered_map>
#include <any>
#include <typeindex>

#include "Classes/Core/GCObject.hpp"

enum class ParamType
{
	Bool,
	Int,
	Float,
	Double,
	String,
};

struct IEventParam : GCObject{};

template<typename T>
struct EventParam : IEventParam
{
	T value;
	template<typename U>
	explicit EventParam(U&& v) : value(std::forward<U>(v))
	{}
};

struct EventParams_auto
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

struct EventParams
{
private:
	std::vector<std::any> values;
	std::unordered_map<std::string,std::any> added_values;
public:
	std::vector<std::type_index> types; // 用 type_index 存类型信息
	EventParams(){}
	EventParams(const std::vector<std::any>& e) : values(e) {
		types.reserve(e.size());
		for (const auto& v : e) {

			types.emplace_back(v.type()); // 自动记录每个值的类型
		}
	}

	template<typename T>
	std::optional<std::decay_t<T>> Get_index(const size_t index) const
	{
		using DecayedT = std::decay_t<T>;

		if (index >= values.size())
		{
			return std::nullopt;
		}
		std::type_index t1 = typeid(T);
		std::type_index t3 = typeid(TEventParams);
		std::type_index t2 = values[index].type();

		if (t3 == t1)
		{
			std::cout << std::endl;
		}
		if (t1 != t2)
		{
			std::cout << std::endl;
		}
		// 尝试 any_cast 为 DecayedT
		if (auto ret = std::any_cast<T>(&values[index]))
		{
			return *ret;
		}

		return std::nullopt;
	}


	void Load(const std::vector<std::any>& e)
	{
		types.clear();
		types.reserve(e.size());
		for (const auto& v : e) {
			types.emplace_back(v.type()); // 自动记录每个值的类型
		}
		values = e;
	}



	template<typename T>
	std::optional<T> Get(const std::string& val_name)
	{
		auto it = added_values.find(val_name);
		if (it != added_values.end())
		{
			if (auto ret = std::any_cast<T>(&it->second))
			{
				return *ret;
			}
		}
		return std::nullopt;
	}
	template<typename U>
	void Add(const std::string& val_name, U&& v)
	{
		//using W = std::decay_t<U>;
		//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
		added_values[val_name] = std::forward<U>(v);
	}
	template<typename U>
	void Add(const std::string& val_name, U& v)
	{
		//using W = std::decay_t<U>;
		//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
		added_values[val_name] = v;
	}
	// //添加变量
	// template<typename U>
	// void Add(const std::string& val_name, U&& v)
	// {
	// 	//using W = std::decay_t<U>;
	// 	//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
	// 	values[val_name] = std::forward<U>(v);
	// }
	// template<typename U>
	// void Add(const std::string& val_name, U& v)
	// {
	// 	//using W = std::decay_t<U>;
	// 	//values[val_name] = make_GCPtr<IEventParam>(static_cast<IEventParam*>(new EventParam<W>(v)));
	// 	values[val_name] = v;
	// }
};