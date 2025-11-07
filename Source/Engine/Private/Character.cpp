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
    capsule->GetPhysicsBody()->quality = 500.f;
    capsule->NativeSetSceneComponentSize(sprite->GetSize());
    SetActorTransform(transform);
}

void Character::EventBegin()
{
    Pawn::EventBegin();
    SetCharacterMoveStrategy(Simulation);
    ListenDispatcher(World()->GetController(),"Key_1",Event([this](TEventParams) {
        std::cout << "切换到上帝模式" << std::endl;
        SetCharacterMoveStrategy(God);
    }));
    ListenDispatcher(World()->GetController(),"Key_2",Event([this](TEventParams) {
        std::cout << "切换到模拟模式" << std::endl;
        SetCharacterMoveStrategy(Simulation);
    }));
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
}


