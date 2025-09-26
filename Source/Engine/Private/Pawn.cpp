#include "Classes/Pawn.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include <iostream>
Pawn::Pawn()
{
	enhanced_input_sub_system = ConstructObjectFromClass(new EnhancedInputSubSystem());

	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_W,[this](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
				player_input_Vec+=UpDir*1.f;
				break;
			case EnhancedInputParamStatus::Start:
				printf("%s\n",eip.input_action.key_name.c_str());
				break;
			case Cancel:
				break;
			case Complete:
				break;
			default: break;
		}
	});
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_S,[this](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
				player_input_Vec+=DownDir*1.f;
				break;
			case EnhancedInputParamStatus::Start:
				printf("%s\n",eip.input_action.key_name.c_str());
				break;
			case Cancel:
				break;
			case Complete:
				printf("%s %s\n",eip.input_action.key_name.c_str(),"Complete");
				break;
			default: break;
		}
	});
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_A,[this](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
				player_input_Vec+=LeftDir*1.f;
				break;
			case EnhancedInputParamStatus::Start:
				printf("%s\n",eip.input_action.key_name.c_str());
				break;
			case Cancel:
				break;
			case Complete:
				break;
			default: break;
		}
	});
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_D,[this](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
				player_input_Vec+=RightDir*1.f;
				break;
			case EnhancedInputParamStatus::Start:
				printf("%s\n",eip.input_action.key_name.c_str());
				break;
			case Cancel:
				break;
			case Complete:
				break;
			default: break;
		}
	});
}

Pawn::~Pawn()= default;

void Pawn::Construct()
{
	Actor::Construct();
	base_move_speed = 200.f;
	name = "ix";
	event_system.AddEvent(Event("test",[this](std::optional<EventParams> event_params) {
		std::cout << name << std::endl;
	}));
}

void Pawn::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);

	this->AddWorldLocation(player_input_Vec.Normalize() * static_cast<float>(deltaTime) * base_move_speed);
	player_input_Vec = {};
	event_system.CallEvent("test");
}

void Pawn::CallEnhancedInputEventBool(EnhancedInputParam<bool> param)
{
	enhanced_input_sub_system->ExecuteBool(param);
}

void Pawn::CallEnhancedInputEventDouble(EnhancedInputParam<double> param) {}
