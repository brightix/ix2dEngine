#pragma once
#include "Classes/Component/SenceComponent/SceneComponent.hpp"
#include "Classes/Core/SPhysics/SPhysicsUtilityBase.hpp"

class RootComponent : public SceneComponent
{
public:
    RootComponent();

    explicit RootComponent(const Transform& transform);
};
