#pragma once
#include <cstdint>

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
}


//inline GameWorld* GetWorld();
