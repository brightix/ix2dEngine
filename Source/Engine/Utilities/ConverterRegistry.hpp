#pragma once
#include <typeindex>


class ConverterRegistry 
{
    using Converter = std::function<std::any(const std::any&)>;
    std::map<std::pair<std::type_index,std::type_index>,Converter> table;
public:
    ConverterRegistry() {}
    template<typename From,typename To>
    void Register(std::function<To(const From&)> func)
    {
        table[{typeid(From), typeid(To)}] = [=](const std::any& v) {
            return std::any(func(std::any_cast<From>(v)));
        };
    }
    template<typename To>
    To Convert(const std::any& v) {
        auto it = table.find({std::type_index(v.type()), std::type_index(typeid(To))});
        if (it != table.end()) {
            return std::any_cast<To>(it->second(v));
        }
        throw std::runtime_error("不支持的转换");
    }
    ~ConverterRegistry()= default;
};

