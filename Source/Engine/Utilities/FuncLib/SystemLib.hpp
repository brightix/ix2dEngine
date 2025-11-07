#pragma once
#include <array>
#include <iostream>
#include <format>
#include <intrin.h>
#include <set>
#include <string>
#include <vector>
class GameWorld;

namespace ix
{
	constexpr static uint32_t Hash(const char* str)
	{
		uint32_t hash = 2166136261u;
		while (*str)
		{
			hash ^= static_cast<uint8_t>(*str++);
			hash *= 16777619u;
		}
		return hash;
	}
	template<typename T, typename U>
	void ConvertSetToVector(const std::set<T,U>& s, std::vector<T>& v)
	{
		for (auto& item : s)
		{
			v.emplace_back(item);
		}
	}


	inline std::string to_string(double val, int reserve_p)
	{
		return std::format("{:.{}f}", val, reserve_p);
	}



}
/**
 *
 * @tparam T Base
 * @tparam U Derived
 * @param other 动态检测对象
 * @param func_name
 * @return 返回是否继承
 */
template<typename T, typename U>
T* IsDerived(U* other = nullptr, const char* func_name = "")
{
	static_assert(std::is_base_of_v<T, U>,"IsChild 检测出 类不继承");
	if (!std::is_base_of_v<T, U>)
	{
		std::cerr << func_name << typeid(U).name() << " 不继承自 " << typeid(T).name() << "\n";
		return nullptr;
	}
	return dynamic_cast<T*>(other);
}

inline std::string GetCPUName()
{
	std::array<int, 4> cpui;
	std::vector<std::array<int, 4>> data;
	__cpuid(cpui.data(), 0);
	int nIds = cpui[0];
	for (int i = 0; i <= nIds; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data.push_back(cpui);
	}

	char name[0x40] = {0};
	if (nIds >= 0x16)
	{
		__cpuid((int*)cpui.data(), 0x80000002);
		memcpy(name, cpui.data(), sizeof(cpui));
		__cpuid((int*)cpui.data(), 0x80000003);
		memcpy(name + 16, cpui.data(), sizeof(cpui));
		__cpuid((int*)cpui.data(), 0x80000004);
		memcpy(name + 32, cpui.data(), sizeof(cpui));
	}
	return std::string(name);
}