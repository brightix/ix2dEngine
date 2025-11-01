#include "../../../Classes/Component/SenceComponent/CollisionBox.hpp"
#include <SDL3/SDL_render.h>
#include "Classes/Actor.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
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

	dispatcher_system.AddEventDispatcher("OnCollision");

	//物理组件
	physics_body = NewObject(new SPhysicsBaseUtility());



	physics_body->SetOwner(this);
	dispatcher_system.AddEventDispatcher("OnEffectTransform");

	physics_body->SetPhysicsCallback([this]() {
		EventParams e;
		e.Add<Location>("location",physics_body->after_location);
		dispatcher_system.CallDispatcher("OnEffectTransform", e);
	});

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
	physics_body->BindEventToDispatcher(this,"OnCollision",Event([this](TEventParams e) {
		dispatcher_system.CallDispatcher("OnCollision", e);
	}));
	ListenDispatcher(owner, "OnMobilityChanged", Event([this](TEventParams e) {
		if (owner->GetMobility() == ActorMobility::Static)
		{
			physics_body->type = PhysicsType::Static;
		}
		else
		{
			physics_body->type = PhysicsType::Movable;
		}
	}));
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
	if (auto t = GetSceneComponentByName("CollisionBoxOutline"))
	{
		t.Cast<StaticTexture>()->SetNewTexture(Create_OutLineTexture_S({w,h}));;
	}
	physics_body->SetBodyBox(size);
}

void CollisionBox::OfferRenderData(std::vector<RenderData>& data)
{
	//SceneComponent::OfferRenderData(data);
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


