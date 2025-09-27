#include "Classes/Actor.h"
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
}

void Actor::Tick(double deltaTime)
{
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

void Actor::AddToWorld(Actor* a) const
{
	game_world->AddToWorld(a);
}
