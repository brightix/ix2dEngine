#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"

CollisionBox::CollisionBox()
{
}

void CollisionBox::Construct()
{
    SceneComponent::Construct();
}

void CollisionBox::ComponentRender()
{
    SDL_Renderer* renderer = GameEngine::Instance().GetRenderer();
    FRect rect(transform.location.x,transform.location.y, w, h);
	SDL_Texture* texture_T = Texture::CreateOutLineTexture(rect);
	StaticTexture t;
	t.SetStaticTexture(texture_T);
	t.SetComponentWorldLocation(transform.location);
	t.ComponentRender();
}

void CollisionBox::SetBoundBox(Vec2<float> size)
{
    collision_box.w = size.x;
    collision_box.h = size.y;
	w = size.x;
	h = size.y;
}


