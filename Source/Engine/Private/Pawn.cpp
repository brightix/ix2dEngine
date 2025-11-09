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

void Pawn::Tick(const double deltaTime)
{
    Actor::Tick(deltaTime);
}

void Pawn::Possessed(Controller *possessed_controller)
{
	// ListenDispatcher(possessed_controller,"",Event([](TEventParams e) {
	//
	// }));
	Log("控制到 " + name);
}

void Pawn::UnPossessed(Controller *possessed_controller)
{

}
void Pawn::RegisterEvents()
{
	Actor::RegisterEvents();

	AddCustomEvent(Event("Possessed",[](TEventParams e) {

	}));
}

void Pawn::CallEnhancedInputEventBool(EnhancedInputParam<bool> param) const
{
	enhanced_input_sub_system->ExecuteBool(std::move(param));
}
void Pawn::CallEnhancedInputEventDouble(EnhancedInputParam<double> param) {}

EnhancedInputSubSystem* Pawn::GetEnhancedInputSubSystem() const
{
	return enhanced_input_sub_system.Get();
}

Vec2<float> Pawn::GetVelocity_Debug() const
{
	return Cast<StaticTextureComponent>(GetSceneComponent("default_texture"))->GetPhysicsBody()->velocity;
}
