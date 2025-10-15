#pragma once
#include "SceneComponent.hpp"
#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

class CollisionBox : public SceneComponent, public MovableUtility
{
public:
    CollisionBox();
    ~CollisionBox(){}
    void Construct() override;
    void ComponentRender() override;
    void SetBoundBox(Vec2<float> size);
};
