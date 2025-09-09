#include "Classes/Pawn.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include <iostream>
Pawn::Pawn()
{
	enhanced_input_sub_system = SpawnActorFromSelf(new EnhancedInputSubSystem());

	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_W,[](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
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
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_S,[](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
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
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_A,[](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
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
	enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_D,[](EnhancedInputParam<bool> eip) {
		auto f = eip.status;
		switch (f)
		{
			case Triggered:
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

void Pawn::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);

}

void Pawn::CallEnhancedInputEventBool(EnhancedInputParam<bool> param)
{
	enhanced_input_sub_system->ExecuteBool(param);
}

void Pawn::CallEnhancedInputEventDouble(EnhancedInputParam<double> param) {}
