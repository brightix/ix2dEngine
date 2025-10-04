#include "Classes/Actor.hpp"

#include <Classes/ActorComponent/ActorComponent.hpp>

#include "../System/GameEngine.hpp"

Actor::Actor() : Actor(Transform()) {}
Actor::Actor(Transform tf) : transform(tf), isShowInGame(false), renderer(nullptr), window(nullptr) {}

Actor::~Actor() = default;

void Actor::Construct()
{
	static_texture = ConstructObjectFromClass(new StaticTexture({120,120},{255,255,255,255}));
}

void Actor::EventBegin()
{
	game_world = make_GCPtr<GameWorld>(GameEngine::Instance().GetGameWorld().Get());
	//game_world = GameEngine::Instance().GetGameWorld().Get();
}

//Actor tick -> component tick
//所有重写tick都在PreTick内
void Actor::Tick(double delta_time) {}

void Actor::ActorComponentTick(double delta_time)
{
	for (auto& c : components)
	{
		c.second->ActorComponentTick(delta_time);
	}
}


void Actor::DestroyActor()
{
	is_pending_kill = true;
	is_active = false;
}

void Actor::AddWorldLocation(Vec2d<float> dis)
{
	transform.Location+=dis;
}

Vec2d<float> Actor::GetWorldLocation() const
{
	return transform.Location;
}

Vec2d<float> Actor::GetRelativeLocation()
{
	return {};
}

void Actor::RenderOnScreen()
{
	// auto& i = GameEngine::Instance();
	// auto t = static_texture;

	GameEngine::Instance().RenderTexture(static_texture,{transform.Location.x,transform.Location.y,static_texture->w,static_texture->h});
}

bool Actor::IsActive() const
{
	return is_active;
}

void Actor::AddToWorld(Actor* a) const
{
	game_world->AddToWorld(a);
}
