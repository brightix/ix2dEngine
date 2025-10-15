#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Core/GameEngine.hpp"

CollisionBox::CollisionBox()
{
}

void CollisionBox::Construct()
{
    SceneComponent::Construct();
}

void CollisionBox::Render()
{
    SDL_Renderer* renderer = GameEngine::Instance().GetRenderer();
    SDL_FRect rect = SDL_FRect(transform.location.x,transform.location.y, w, h);
    SDL_Texture =
    SDL_RenderTexture(renderer,);
}

void CollisionBox::SetBoundBox(Vec2<float> size)
{
    collision_box.rect.w = size.x;
    collision_box.rect.h = size.y;
}


