#pragma once
#include "Asset.hpp"
#include <SDL3/SDL.h>
class StaticTexture : public Asset
{
    std::shared_ptr<SDL_Texture> texture;
public:
    StaticTexture()= default;
};
