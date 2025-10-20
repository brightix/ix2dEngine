#pragma once
#include <string>
#include <typeinfo>

template<typename Derived>
class NameHelper
{
public:
	std::string name;
	NameHelper() { name = typeid(Derived).name(); } // 或者用宏/静态方法美化名字
};