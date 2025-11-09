#pragma once
#include "Classes/Actor.hpp"

class StaticTextureComponent;

class TestGround : public Actor
{
    GCPtr<StaticTextureComponent> st;
public:
	Vec2<float> size;
    using Actor::Actor;
    void Construct() override;

    StaticTextureComponent* GetGroundTexture();
    void SetGroundSize(const Vec2<float>& new_size);
};
