#include "Classes/Character.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Component/SenceComponent/Capsule2d.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Types/Enums/PivotDir.hpp"
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
	capsule->SetComponentPivot(PivotDir::BOTTOM_CENTER);
	move_strategy = God;
}

void Character::EventBegin()
{
    Pawn::EventBegin();
    SetCharacterMoveStrategy(Simulation);
	//ListenDispatcher(World()->GetController(),"Key_3",&Character::Test);
}

void Character::RegisterDispatchers()
{
	Pawn::RegisterDispatchers();
	AddDispatcher("OnMoveStrategyChanged",{TypeID(bool)});
}

void Character::RegisterEvents()
{
	Pawn::RegisterEvents();
	AddCustomEvent(Event("MoveStrategyChanged",[this](const bool pressed) {
		if (pressed)
		{
			auto current_id = static_cast<CharacterMoveStrategy>(++move_strategy % MoveStrategySize);
			std::cout << magic_enum::enum_name<CharacterMoveStrategy>(current_id) << std::endl;
			SetCharacterMoveStrategy(current_id);
		}
	}));
}


void Character::Possessed(Controller* possessed_controller)
{
	Pawn::Possessed(possessed_controller);
	sprite->SetStaticTexture(Create_FilledTexture_S({50,50},RED));
	ListenDispatcher(World()->GetController(),"Key_1", "MoveStrategyChanged");
}

void Character::UnPossessed(Controller* possessed_controller)
{
	Pawn::UnPossessed(possessed_controller);
	sprite->SetStaticTexture(Create_FilledTexture_S({50,50},BLUE));
	IgnoreDispatcher(possessed_controller,"Key_1");
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
    return static_cast<CharacterMoveStrategy>(move_strategy % MoveStrategySize);
}

void Character::SetCharacterMoveStrategy(CharacterMoveStrategy new_strategy)
{
    move_strategy = new_strategy;
	if (new_strategy == God)
	{
		Root->IgnoreDispatcher(capsule->GetPhysicsBody(),"OnSynchronization");
	}
	else if (new_strategy == Simulation)
	{
		Root->GetPhysicsBody()->velocity = {};
		Root->ListenDispatcher(capsule->GetPhysicsBody(),"OnSynchronization","Synchronization");
	}
	CallDispatcher("OnMoveStrategyChanged");
}

void Character::SetCharacterOpenPhysics(PhysicsType new_physics) const
{
	capsule->NativeSetPhysicsType(new_physics);
}

int Character::Test(bool b)
{
	std::cout << "Test" << std::endl;
	return b;
}


