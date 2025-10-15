#include "Classes/Pawn.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include <iostream>
#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include <utility>

#include "Classes/Controller.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Enum/ActorEnum.hpp"

Pawn::Pawn() : Actor(Transform()), base_move_speed(200.f) {}

Pawn::~Pawn()= default;

void Pawn::Construct()
{
	Actor::Construct();
	enhanced_input_sub_system = NewObject(new EnhancedInputSubSystem());
	actor_components["movable"] = NewObject(new MovableComponent(this));
	base_move_speed = 200.f;
	name = "ix";
	event_system.AddEvent(Event("test",[this](std::optional<EventParams> event_params) {
		std::cout << name << std::endl;
	}));

	// dispatcher_system.AddEventDispatcher("EnhancedInput_CallBool");
	// dispatcher_system.BindEventTo("EnhancedInput_CallBool",this,Event("pawn",[this](std::optional<EventParams> e) {
	// 	enhanced_input_sub_system->ExecuteBool(std::move(param));
	// }));
	dispatcher_system.AddEventDispatcher("TestDispatcher");
	dispatcher_system.BindEventTo("TestDispatcher",this,Event("pawn",[this](std::optional<EventParams> e) {
		std::cout << *e->Get<std::string>("name") << std::endl;
	}));
	mobility = ActorMobility::Movable;

	// BindNormalKeyEvent("Key_R", this, Event("Key_W", [](TEventParams e) {
	// 	std::cout << std::string("Normal Key ") + "R" << std::endl;
	// }));
	AddSceneComponent<CollisionBox>("碰撞箱",new CollisionBox());
	GetSceneComponent<CollisionBox>("碰撞箱")->SetBoundBox(GetSceneComponent<StaticTexture>("DefaultTexture")->GetSize());
}

void Pawn::EventBegin()
{
	Actor::EventBegin();
}

void Pawn::Tick(double deltaTime)
{
    Actor::Tick(deltaTime);
	//std::cout << transform.location.str() << std::endl;
	//this->AddWorldLocation(player_input_Vec.Normalize() * static_cast<float>(deltaTime) * base_move_speed);
	//player_input_Vec = {};
	//event_system.CallEvent("test");
	// EventParams eip;
	// eip.Add<std::string>("name","Fuck");
	//dispatcher_system.CallEvent("TestDispatcher",eip);
}

void Pawn::CallEnhancedInputEventBool(EnhancedInputParam<bool> param)
{
	enhanced_input_sub_system->ExecuteBool(std::move(param));
}
void Pawn::CallEnhancedInputEventDouble(EnhancedInputParam<double> param) {}

EnhancedInputSubSystem* Pawn::GetEnhancedInputSubSystem()
{
	return enhanced_input_sub_system.Get();
}
