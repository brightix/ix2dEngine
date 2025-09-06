#pragma once
#include <algorithm>
#include <memory>
#include <type_traits>
#include <unordered_map>

#include "Window/GlobalMacros.hpp"

enum class ParamType
{
    Bool,
    Int,
    Float,
    Double,
    String,
};

struct IEventParam { virtual ~IEventParam() = default; };

template<typename T>
struct EventParam : IEventParam
{
    T value;
    template<typename U>
    explicit EventParam(U&& v) : value(std::forward<U>(v))
    {
    }
};

using EventParamXx = std::unordered_map<std::string,sptr<IEventParam>>;

struct EventParams
{
    std::unordered_map<std::string,sptr<IEventParam>> values;
    template<typename T>
    std::optional<T> Get(const std::string& val_name)
    {
        auto it = values.find(val_name);
        if (it != values.end())
        {
            return static_cast<EventParam<T>*>(it->second.get())->value;
        }
        return std::nullopt;
    }
    //添加变量
    template<typename U>
    void Add(const std::string& val_name, U&& v)
    {
        using W = std::decay_t<U>;  // 去掉引用、cv修饰，得到值类型
        values[val_name] = std::make_shared<EventParam<W>>(std::forward<U>(v));
    }
};

//
// struct EventParams
// {
//     ParamType type;
//     union EventValue
//     {
//         bool b;
//         int i;
//         float f;
//         double d;
//         const char* s;
//     };
// };
//
// inline GetEventParam()
// {
//
// }

