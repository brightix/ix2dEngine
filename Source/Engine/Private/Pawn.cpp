#include "Classes/Pawn.hpp"
#include <utility>


#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include "Classes/Controller.hpp"
#include "Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
Pawn::~Pawn()= default;

void Pawn::Construct()
{
	Actor::Construct();
	enhanced_input_sub_system = NewObject<EnhancedInputSubSystem>(this);
	actor_components.emplace("movable",NewObject<MovableComponent>(this));

	base_move_speed = 200.f;
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

void Pawn::Possessed(Controller *possessed_controller)
{
	// ListenDispatcher(possessed_controller,"",Event([](TEventParams e) {
	//
	// }));
}

void Pawn::UnPossessed(Controller *possessed_controller)
{
	// ListenDispatcher(possessed_controller,"",Event([](TEventParams e) {
	//
	// }));
}
void Pawn::RegisterEvents()
{
	Actor::RegisterEvents();

	AddCustomEvent(Event("Possessed",[](TEventParams e) {

	}));
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

Vec2<float> Pawn::GetVelocity_Debug() const
{
	return Cast<StaticTextureComponent>(GetSceneComponent("default_texture"))->GetPhysicsBody()->velocity;
}
