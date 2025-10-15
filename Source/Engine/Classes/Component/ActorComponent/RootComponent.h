#pragma once
#include "ActorComponent.hpp"
#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

class RootComponent : public ActorComponent, SPhysicsBaseUtility
{
    RootComponent();
};
