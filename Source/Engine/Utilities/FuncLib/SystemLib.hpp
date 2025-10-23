#pragma once
#include <cstdint>
#include <set>
#include <vector>
#include <SDL3/SDL_pixels.h>
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
}
