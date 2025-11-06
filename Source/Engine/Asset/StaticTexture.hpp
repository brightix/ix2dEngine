#pragma once
#include "Asset.hpp"
#include "../Classes/Component/SenceComponent/SceneComponent.hpp"

class StaticTexture : public Asset
{
    std::shared_ptr<SDL_Texture> texture;
public:
    StaticTexture(){}
};
