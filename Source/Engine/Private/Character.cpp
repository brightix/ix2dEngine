#include "Classes/Character.hpp"

#include "Classes/Component/SenceComponent/Capsule2d.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Enum/ActorEnum.hpp"
#include "Utilities/FuncLib/GlobalMacros.hpp"

class StaticTexture;

Character::Character()
{
    CNAME;
}

void Character::Construct()
{
    Pawn::Construct();
    mobility = ActorMobility::Movable;


    auto cap = NewObject<Capsule2d>(this);
    SceneComponent::Replace(Root.Get(),cap);
    sprite = GetSceneComponent("default_texture").Cast<StaticTexture>();
    sprite->SetNewTexture(Create_FilledTexture_S({100,100}));
    sprite->SetActiveCollision(true);
    sprite->SetPhysicsType(PhysicsType::Movable);


}


