#pragma once
#include "Classes/Actor.hpp"
#include "SDL3/SDL_pixels.h"

class StaticTextureComponent;

class TestGround : public Actor
{
    GCPtr<StaticTextureComponent> st;
public:
	Vec2<float> size;
    using Actor::Actor;
    void Construct() override;

    StaticTextureComponent* GetGroundTexture();
    void SetGroundSizeAndColor(const Vec2<float>& new_size, SDL_Color color = GREEN);
};
