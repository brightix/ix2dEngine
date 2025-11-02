#include "Classes/Component/SenceComponent/StaticTexture.hpp"

#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Types/RenderData.hpp"


StaticTexture::StaticTexture() : simulation_physics(false)
{
	CNAME;
}


StaticTexture::StaticTexture(const StaticTexture& other) : simulation_physics(false)
{
	CNAME;
	//SetStaticTexture(other.in_texture.get());
	in_texture = other.in_texture;
	w = other.w;
	h = other.h;
	pivot = other.pivot;
}

void StaticTexture::Construct()
{
	Texture::Construct();
//具有开关物理属性
	physics_body = NewObject(new SPhysicsBaseUtility());
	physics_body->SetOwner(this);

	in_texture = GameEngine::Instance().GetDefaultTexture();
}

void StaticTexture::ComponentEventBegin()
{
	Texture::ComponentEventBegin();
	ListenDispatcher(physics_body.Get(),"OnSynchronization",Event([this](TEventParams e) {
		//Location after_location = physics_body->after_location;
		SetComponentWorldLocation(physics_body->after_location);
	}));
}


TextureType StaticTexture::GetTextureType()
{
	return TextureType::StaticTexture;
}

GCWeakPtr<SPhysicsBaseUtility> StaticTexture::GetPhysicsBody()
{
	return physics_body;
}

void StaticTexture::ComponentRender()
{
	SDL_Renderer* renderer = GetRenderer();
	const SDL_FRect dst(world_transform.location.x,world_transform.location.y, h, w);
	SDL_RenderTexture(renderer,in_texture.get(),nullptr,&dst);
}

void StaticTexture::SetActiveCollision(const bool is_active, const PhysicsType new_type) const
{
	physics_body->SetSimulationPhysics(is_active, new_type);

	// if (simulation_physics == is_active)
	// 	return;
	// simulation_physics = is_active;
	// if (simulation_physics)
	// {
	// 	physics_body = NewObject(new SPhysicsBaseUtility());
	// 	physics_body->SetOwner(this);
	// }
	// else
	// {
	// 	physics_body->Destroy();
	// 	physics_body.Reset();
	// }
}

void StaticTexture::SetPhysicsType(const PhysicsType new_type) const
{
	physics_body->SetPhysicsType(new_type);
}


void StaticTexture::OfferRenderData(std::vector<RenderData>& data)
{
	data.emplace_back(RenderData(in_texture, world_transform, {},SDL_FRect(world_transform.location.x,world_transform.location.y, w, h), layer));
}
