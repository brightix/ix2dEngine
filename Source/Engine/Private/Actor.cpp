#include "Classes/Actor.hpp"

#include <Classes/ActorComponent/ActorComponent.hpp>

#include "../System/GameEngine.hpp"
#include "Enum/ActorEnum.hpp"

Actor::Actor() : Actor(Transform()) {}
Actor::Actor(const Transform &tf) : transform(tf), isShowInGame(false), is_active(true), mobility(ActorMobility::Static), renderer(nullptr),
                             window(nullptr) {}

Actor::~Actor() = default;

void Actor::Construct()
{
	renderer = GameEngine::Instance().GetRenderer();
	//默认生成一个200x200的矩形作为sprite
	//SDL_Texture* t = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,200,200);
	collision_box = ConstructObjectFromClass(new StaticTexture({200,200},{255,255,255,255}));
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
	//单线程
	// is_pending_kill = true;
	// is_active = false;

	//三缓冲
	is_pre_kill = true;
	is_active = false;
}

void Actor::SetMobility(const ActorMobility target_mobility)
{
	mobility = target_mobility;
}


void Actor::AddWorldLocation(Vec2d<float> dis)
{
	transform.location+=dis;
}

Location Actor::GetWorldLocation() const
{
	return transform.location;
}

Vec2d<float> Actor::GetRelativeLocation()
{
	return {};
}

Transform Actor::GetWorldTransform() const
{
	return transform;
}

void Actor::RenderOnScreen()
{
	// auto& i = GameEngine::Instance();
	// auto t = static_texture;
	//GameEngine::Instance().RenderTexture(collision_box,{transform.location.x,transform.location.y,static_cast<float>(collision_box->w),static_cast<float>(collision_box->h)});
}

void Actor::RenderCollisionBox() const
{
	GameEngine::Instance().RenderTexture(collision_box,SDL_FRect(transform.location.x,transform.location.y,collision_box->w,collision_box->h));
}

bool Actor::IsActive() const
{
	return is_active;
}

void Actor::AddToWorld(Actor* a) const
{
	game_world->AddToWorld(a);
}
