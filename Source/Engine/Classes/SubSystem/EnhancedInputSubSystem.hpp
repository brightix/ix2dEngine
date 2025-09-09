#pragma once
#include <functional>
#include <unordered_map>
#include <SDL3/SDL_scancode.h>

#include "Classes/InputMap.hpp"
#include "Classes/Object.h"
#include "Types/Vec.hpp"

enum EnhancedInputParamStatus
{
	Triggered,
	Start,
	Cancel,
	Complete,
};

template<typename T>
struct EnhancedInputParam
{
	T val;
	EnhancedInputParamStatus status;

	EnhancedInputParam(T v, EnhancedInputParamStatus s = EnhancedInputParamStatus::Start)
		: val(v), status(s) {}

	T Get() const { return val; }
};

// Vec2d<double> 特化
template<>
struct EnhancedInputParam<Vec2d<double>>
{
	Vec2d<double> val;
	EnhancedInputParamStatus status;

	EnhancedInputParam(const Vec2d<double>& v, EnhancedInputParamStatus s = EnhancedInputParamStatus::Start)
		: val(v), status(s) {}

	Vec2d<double> Get() const { return val; }
};

// Vec<double> 特化
template<>
struct EnhancedInputParam<Vec<double>>
{
	Vec<double> val;
	EnhancedInputParamStatus status;

	EnhancedInputParam(const Vec<double>& v, EnhancedInputParamStatus s = EnhancedInputParamStatus::Start)
		: val(v), status(s) {}

	Vec<double> Get() const { return val; }
};

// bool 特化
template<>
struct EnhancedInputParam<bool>
{
	bool val;
	EnhancedInputParamStatus status;
	InputAction input_action;
	EnhancedInputParam(InputAction ia, const bool v, EnhancedInputParamStatus sts)
	{
		val = v;
		status = sts;
		input_action = ia;
	}

	bool Get() const { return val; }
};


class EnhancedInputSubSystem : public Object
{
	std::unordered_map<SDL_Scancode,std::function<void(EnhancedInputParam<bool>)>> enhanced_input_event_bool;
	std::unordered_map<SDL_Scancode,std::function<void(EnhancedInputParam<double>)>> enhanced_input_event_double;
public:
    EnhancedInputSubSystem();
    ~EnhancedInputSubSystem()= default;
	void AddInputEventBool(SDL_Scancode scancode,std::function<void(EnhancedInputParam<bool>)> func);
	void ExecuteBool(EnhancedInputParam<bool> param);
private:
	//bool TryInsertToMap();
};

