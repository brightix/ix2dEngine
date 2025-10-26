#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/SPhysics/SPhysics.hpp"
#include "Utilities/FuncLib/Deleter.hpp"
#include "Types/RenderData.hpp"

CollisionBox::CollisionBox()
{
	NAME;
}

void CollisionBox::Construct()
{
	SceneComponent::Construct();
	physics_body = NewGCPtr(new SPhysicsBaseUtility());
	physics_body->SetOwner(this);
	auto texture = MountedComponent(new StaticTexture);
	texture->SetComponentName("CollisionBoxOutline");
	texture->SetNewTexture(TTexture(RendererCenter::CreateOutLineTexture({w,h})));
	is_outline_visible = true;
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
	if (auto t = GetSceneComponentByName("CollisionBoxOutline").Cast<StaticTexture>())
	{
		t->SetNewTexture(TTexture(RendererCenter::CreateOutLineTexture({w,h})));
	}
}

void CollisionBox::OfferRenderData(std::vector<RenderData>& data)
{
	SceneComponent::OfferRenderData(data);
	if (is_outline_visible)
	{
		auto texture = GetSceneComponentByName("CollisionBoxOutline").Cast<StaticTexture>()->GetTexture();
		auto rd = RenderData(this,texture);
		rd.layer = 3;
		data.emplace_back(std::move(rd));
	}
}


