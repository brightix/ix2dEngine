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
	//物理组件
	physics_body = NewGCPtr(new SPhysicsBaseUtility());
	physics_body->SetOwner(this);

	//默认描边
	auto texture = MountedComponent(new StaticTexture);
	texture->SetComponentName("CollisionBoxOutline");
	texture->SetNewTexture(TTexture(RendererCenter::CreateOutLineTexture({w,h})));
	is_outline_visible = true;


}

void CollisionBox::ComponentEventBegin()
{
	SceneComponent::ComponentEventBegin();
	// BindEvent(this, "OnSceneComponentTeleport",Event([&](TEventParams e) {
	// 	Synchronization();
	// }));
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
	w = size.x;
	h = size.y;
	if (auto t = GetSceneComponentByName("CollisionBoxOutline").Cast<StaticTexture>())
	{
		t->SetNewTexture(TTexture(RendererCenter::CreateOutLineTexture({w,h})));
	}
	physics_body->SetBodyBox(size);
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

void CollisionBox::Synchronization() const
{
	//physics_body->collision_rect.x = transform.location.x;
	//physics_body->collision_rect.y = transform.location.y;
	std::cout << "CollisionBox::Synchronization()" << std::endl;
}


