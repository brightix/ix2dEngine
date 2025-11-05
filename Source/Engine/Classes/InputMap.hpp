#pragma once
#include <string>
#include <unordered_map>
#include <SDL3/SDL_scancode.h>

#include "Object.hpp"
#include "Types/Enums/EMouseCode.hpp"

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

struct MouseAction
{
	std::string button_name;
	MouseCode mouse_code;
	EnhancedInputParamType type;
};

struct InputMap : public Object
{
	std::unordered_map<SDL_Scancode,InputAction> Enhanced;
	std::unordered_map<SDL_Scancode,InputAction> Normal;
	std::unordered_map<Uint8,MouseAction> Mouse;
	InputMap();
	void Construct() override;
};

