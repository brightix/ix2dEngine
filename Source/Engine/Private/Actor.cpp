#include "Classes/Actor.h"
#include "../System/GameEngine.hpp"

Actor::Actor() : Object() {}
Actor::Actor(Transform tf) : transform(tf) {}

Actor::~Actor() {}

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

Vec2d<float> Actor::GetWorldLocation()
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

void Actor::AddToWorld(Actor* a)
{
	game_world->AddToWorld(a);
}
