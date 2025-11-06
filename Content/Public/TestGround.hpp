#pragma once
#include "Classes/Actor.hpp"

class StaticTextureComponent;

class TestGround : public Actor
{
    GCPtr<StaticTextureComponent> st;
public:
    using Actor::Actor;
    void Construct() override;

    void SetGroundSize(const Vec2<float> &new_size) const;
};
