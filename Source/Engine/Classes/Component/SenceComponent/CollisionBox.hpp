#pragma once
#include "SceneComponent.hpp"
#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

class CollisionBox final : public SceneComponent, public MovableUtility
{
public:
    CollisionBox();
    void Construct() override;
    void ComponentRender() override;
    void SetBoundBox(const Vec2<float>& size);
};
