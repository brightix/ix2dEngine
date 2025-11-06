#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"

#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Types/RenderData.hpp"


StaticTextureComponent::StaticTextureComponent()
{
	CNAME;
}


StaticTextureComponent::StaticTextureComponent(const StaticTextureComponent& other)
{
	CNAME;
	//SetStaticTexture(other.in_texture.get());
	s_texture = other.s_texture;
	w = other.w;
	h = other.h;
	pivot = other.pivot;
}

void StaticTextureComponent::Construct()
{
	SceneComponent::Construct();
//具有开关物理属性
	physics_body = NewObject<SPhysicsBaseUtility>(this);
	s_texture = NewObject<Texture>(this);
	//physics_body->SetPhysicsBodyOwner(this);
}

void StaticTextureComponent::ComponentEventBegin()
{
	SceneComponent::ComponentEventBegin();
}



void StaticTextureComponent::ComponentRender()
{
	SDL_Renderer* renderer = GetRenderer();
	const SDL_FRect dst(world_transform.location.x,world_transform.location.y, h, w);
	SDL_RenderTexture(renderer,s_texture->GetTexture().get(),nullptr,&dst);
}

Vec2<float> StaticTextureComponent::GetSize()
{
	return s_texture->GetSize();
}

void StaticTextureComponent::SetStaticTexture(std::shared_ptr<SDL_Texture> new_texture)
{
	if (new_texture)
	{
		w = new_texture->w;
		h = new_texture->h;
		s_texture->SetTexture(new_texture);
	}
	else
	{
		w = 0;
		h = 0;
	}
}

Texture* StaticTextureComponent::GetTexture() const
{
	return s_texture.Get();
}

std::shared_ptr<SDL_Texture> StaticTextureComponent::GetRowTexture() const
{
	return s_texture->GetTexture();
}


void StaticTextureComponent::OfferRenderData(std::vector<RenderData>& data)
{
	data.emplace_back(RenderData(s_texture->GetTexture(), world_transform, {},SDL_FRect(world_transform.location.x,world_transform.location.y, w, h), layer));
}
