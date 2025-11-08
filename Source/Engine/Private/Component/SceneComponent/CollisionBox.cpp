#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"
#include <SDL3/SDL_render.h>
#include "Classes/Actor.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/SPhysics/SPhysics.hpp"
#include "Enum/ActorEnum.hpp"
#include "Types/RenderData.hpp"

CollisionBox::CollisionBox()
{
	CNAME;
}

void CollisionBox::Construct()
{
	SceneComponent::Construct();

	dispatcher_system.AddDispatcher("OnCollision");

	//物理组件
	physics_body = NewObject<SPhysicsBaseUtility>(this);
	AddDispatcher("OnEffectTransform",{typeid(Transform)});


	//physics_body->SetBodyBox({w,h});

	//默认描边
	// auto texture = MountedComponent(new StaticTexture);
	// texture->SetName("CollisionBoxOutline");
	// texture->SetNewTexture(Create_OutLineTexture_S({w,h}));


	is_outline_visible = false;
}

void CollisionBox::ComponentEventBegin()
{
	SceneComponent::ComponentEventBegin();
	ListenDispatcher(physics_body.Get(),"OnCollision", "Collision");
	ListenDispatcher(physics_body.Get(),"OnSynchronization", "CollisionSynchronization");
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
	if (auto t = GetSceneComponentByName("CollisionBoxOutline"))
	{
		Cast<StaticTextureComponent>(t)->SetStaticTexture(Create_OutLineTexture_S({w,h}));;
	}
	//physics_body->SetBodyBox(size);
}

void CollisionBox::OfferRenderData(std::vector<RenderData>& data)
{
	//SceneComponent::OfferRenderData(data);
	if (is_outline_visible)
	{
		const auto texture = Cast<StaticTextureComponent>(GetSceneComponentByName("CollisionBoxOutline"))->GetRowTexture();
		auto rd = RenderData(this,texture);
		rd.layer = 3;
		data.emplace_back(std::move(rd));
	}
}



