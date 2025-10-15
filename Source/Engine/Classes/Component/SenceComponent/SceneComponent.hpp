#pragma once
#include "Classes/Component/Component.hpp"
#include "Types/Transform.hpp"

class SceneComponent : public Component
{
protected:
    Transform transform;
    int w;
    int h;
    Vec2<float> pivot;
public:
    SceneComponent();
    ~SceneComponent();
    virtual void Render(){}
};
