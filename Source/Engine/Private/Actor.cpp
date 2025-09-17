#include "Classes/Actor.h"
#include "../System/GameEngine.hpp"

void Actor::AddWorldLocation(Vec2d<float> dis)
{
	transform.Location+=dis;
}

Vec2d<float> Actor::GetWorldLocation()
{
	return {};
}

Vec2d<float> Actor::GetRelativeLocation()
{
	return {};
}

void Actor::RenderOnScreen()
{
	GameEngine::Instance().RenderTexture(static_texture,{transform.Location.x,transform.Location.y,static_texture->w,static_texture->h});
}

void Actor::Construct()
{
    //renderer = GameEngine::Instance().GetRenderer();
}
Actor::Actor() : Object()
{
	game_world = make_GCPtr<GameWorld>(GameEngine::Instance().GetGameWorld().Get());
	Actor::Construct();
}

Actor::Actor(Transform tf)
{
    transform = tf;
    Actor::Construct();
}

Actor::~Actor() {}
void Actor::EventBegin()
{
	static_texture = SpawnActorFromSelf(new StaticTexture({120,120},{255,255,255,255}));
}
