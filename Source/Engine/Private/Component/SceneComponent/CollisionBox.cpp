#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"

CollisionBox::CollisionBox()= default;

void CollisionBox::Construct()
{
    SceneComponent::Construct();
	auto texture = NewObject<StaticTexture>(new StaticTexture);
	//RendererCenter::AsyncLoadOutLine();
	mounted_components.emplace("OutLine", texture);
}


void CollisionBox::ComponentRender()
{
 //    //SDL_Renderer* renderer = GetRenderer();
 //    FRect rect(transform.location.x,transform.location.y, w, h);
	// SDL_Texture* texture_T = Texture::CreateOutLineTexture(rect);
	// StaticTexture t;
	// t.SetStaticTexture(texture_T);
	// t.SetComponentWorldLocation(transform.location);
	// t.ComponentRender();
}

void CollisionBox::SetBoundBox(const Vec2<float>& size)
{
	w = static_cast<int>(size.x);
	h = static_cast<int>(size.y);
}


