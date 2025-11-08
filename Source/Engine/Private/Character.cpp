#include "Classes/Character.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Component/SenceComponent/Capsule2d.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Utilities/FuncLib/GlobalMacros.hpp"

void Character::Construct()
{
    Pawn::Construct();

    CNAME;
    capsule = NewObject<Capsule2d>(this);

    SceneComponent::Replace(Root.Get(),capsule);
    sprite = Cast<StaticTextureComponent>(GetSceneComponent("default_texture"));
    sprite->SetStaticTexture(Create_FilledTexture_S({50,50},BLUE));
    capsule->NativeSetActiveCollision(true);
    capsule->SetComponentName("capsule");
    capsule->GetPhysicsBody()->mass = 200.f;
    capsule->NativeSetSceneComponentSize(sprite->GetSize());
    SetActorTransform(transform);
}

void Character::EventBegin()
{
    Pawn::EventBegin();
    SetCharacterMoveStrategy(Simulation);
	ListenDispatcher_Lambda(World()->GetController(),"Key_1", [this](const bool pressed) {
		if (pressed)
		{
			std::cout << "切换到上帝模式" << std::endl;
			SetCharacterMoveStrategy(God);
		}
	});
    ListenDispatcher(World()->GetController(),"Key_2","ToGodMode");
	//ListenDispatcher(World()->GetController(),"Key_3","Test");
	ListenDispatcher(World()->GetController(),"Key_3",&Character::Test);
	//dispatcher_system.DelegateEvent(this, "OnTest", "Test");
}

void Character::RegisterDispatchers()
{
	Pawn::RegisterDispatchers();
	AddDispatcher("OnMoveStrategyChanged");
	AddDispatcher("OnTest");
}

void Character::RegisterEvents()
{
	Pawn::RegisterEvents();
	AddCustomEvent(Event("Test",[](TEventParams e) {
		std::cout << "Do Test" << std::endl;
	}));
	AddCustomEvent("ToGodMode",Event("GodMode", [this](const bool pressed) {
		if (pressed)
		{
			std::cout << "切换到上帝模式" << std::endl;
			SetCharacterMoveStrategy(God);
		}
	}));
	AddCustomEvent(Event("ToSimMode",[this](TEventParams e) {
		std::cout << "切换到模拟模式" << std::endl;
		SetCharacterMoveStrategy(Simulation);
	}));



}


void Character::Possessed(Controller* possessed_controller)
{
	Pawn::Possessed(possessed_controller);
	sprite->SetStaticTexture(Create_FilledTexture_S({50,50},RED));
}

void Character::UnPossessed(Controller* possessed_controller)
{
	Pawn::UnPossessed(possessed_controller);
	sprite->SetStaticTexture(Create_FilledTexture_S({50,50},BLUE));
}

SPhysicsBaseUtility * Character::GetCharacterPhysicsBody() const
{
    if (auto p = Cast<Capsule2d>(Root.Get()))
    {
        return p->GetPhysicsBody();
    }
    return nullptr;
}

CharacterMoveStrategy Character::GetCharacterMoveStrategy() const
{
    return strategy;
}

void Character::SetCharacterMoveStrategy(CharacterMoveStrategy new_strategy)
{
    strategy = new_strategy;
	CallDispatcher("OnMoveStrategyChanged");
}

int Character::Test(bool b)
{
	std::cout << "Test" << std::endl;
	return b;
}


