#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_scancode.h>

#include "Object.hpp"

enum class EnhancedInputParamType
{
	Bool,
	Double,
	Vec2d,
	Vec
};

struct InputAction
{
	std::string key_name;
	SDL_Scancode scancode;
	EnhancedInputParamType type;
};

struct InputMap : public Object
{
	std::unordered_map<SDL_Scancode,InputAction> Enhanced;
	std::unordered_map<SDL_Scancode,InputAction> Normal;
	InputMap();
	void Construct() override;
};

