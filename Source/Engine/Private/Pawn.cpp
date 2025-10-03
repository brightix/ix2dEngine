#include "Classes/Pawn.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include <iostream>
#include <Classes/ActorComponent/MovableComponent.hpp>

Pawn::Pawn()
{
}

Pawn::~Pawn()= default;

void Pawn::Construct()
{
	Actor::Construct();
	enhanced_input_sub_system = ConstructObjectFromClass(new EnhancedInputSubSystem());
	components["movable"] = ConstructObjectFromClass(new MovableComponent(this));
	base_move_speed = 200.f;
	name = "ix";
	event_system.AddEvent(Event("test",[this](std::optional<EventParams> event_params) {
		std::cout << name << std::endl;
	}));
	dispatcher_system.AddEventDispatcher("TestDispatcher");
	dispatcher_system.BindEventTo("TestDispatcher",this,Event("pawn",[this](std::optional<EventParams> e) {
		std::cout << *e->Get<std::string>("name") << std::endl;
	}));
}

void Pawn::EventBegin()
{
	Actor::EventBegin();
}

void Pawn::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);

	//this->AddWorldLocation(player_input_Vec.Normalize() * static_cast<float>(deltaTime) * base_move_speed);
	//player_input_Vec = {};
	//event_system.CallEvent("test");
	// EventParams eip;
	// eip.Add<std::string>("name","Fuck");
	//dispatcher_system.CallEvent("TestDispatcher",eip);
}

void Pawn::CallEnhancedInputEventBool(EnhancedInputParam<bool> param)
{
	enhanced_input_sub_system->ExecuteBool(param);
}
void Pawn::CallEnhancedInputEventDouble(EnhancedInputParam<double> param) {}
EnhancedInputSubSystem* Pawn::GetEnhancedInputSubSystem()
{
	return enhanced_input_sub_system.Get();
}
