#pragma once
#include <iostream>
#include <format>
#include <set>
#include <string>
#include <vector>
#define ASSERT_CHILD(Base,Derived) ix::IsChild<(Base),(Derived)>(__func__)
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

	/**
	 *
	 * @tparam T Base
	 * @tparam U Derived
	 * @param other 动态检测对象
	 * @param func_name
	 * @return 返回是否继承
	 */
	template<typename T, typename U>
	bool IsChild(U* other = nullptr, const char* func_name = "")
	{
		static_assert(std::is_base_of_v<T, U>,"IsChild 检测出 类不继承");
		if (!std::is_base_of_v<T, U>)
		{
			std::cerr << func_name << typeid(U).name() << " 不继承自 " << typeid(T).name() << "\n";
			return false;
		}
		return dynamic_cast<T*>(other) != nullptr;
	}
}

