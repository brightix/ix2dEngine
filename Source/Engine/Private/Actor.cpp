#include "Classes/Actor.hpp"

#include "Classes/Component/ActorComponent/ActorComponent.hpp"
#include "Classes/Component/ActorComponent/RootComponent.h"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Enum/ActorEnum.hpp"

Actor::Actor() : Actor(Transform()){}
Actor::Actor(const Transform &tf) : isShowInGame(true), is_active(true), hidden_in_game(false),
                                    window(nullptr),
                                    mobility(ActorMobility::Static) { }

Actor::~Actor() = default;

void Actor::Construct()
{
	NAME;
	//场景默认根组件
	Root = NewObject<SceneComponent>(new RootComponent(Transform{{0,0}}));
	auto default_image = NewObject(new StaticTexture());
	default_image->name = "default_texture";
	Root->MountedComponent(default_image);
}

void Actor::EventBegin()
{
	game_world = make_GCPtr<GameWorld>(GameEngine::Instance().GetGameWorld().Get());
	//game_world = GameEngine::Instance().GetGameWorld().Get();
}

//Actor tick -> component tick
//所有重写tick都在PreTick内
void Actor::Tick(double delta_time) {}

void Actor::RootComponentTick(double delta_time)
{
	for (auto& it : actor_components)
	{
		it.second->ActorComponentTick(delta_time);
	}
	Root->SceneComponentTick(delta_time);
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


void Actor::AddActorWorldLocation(Vec2<float> dis) const
{
	Root->AddComponentWorldLocation(dis);
}

Location Actor::GetWorldLocation() const
{
	return Root->GetComponentTransform().location;
}

Vec2<float> Actor::GetRelativeLocation()
{
	return {};
}

Transform Actor::GetWorldTransform() const
{
	return Root->GetComponentTransform();
}

void Actor::RenderOnScreen()
{
	Root->ForRender();
	// for (auto& it : scene_components)
	// {
	// 	it.second->ComponentRender();
	// }
}

void Actor::ForRenderOrder(std::vector<RenderData>& data) const
{
	Root->ForRenderData(data);
}

// void Actor::RenderCollisionBox() const
// {
// 	GameEngine::Instance().RenderTexture(collision_box,SDL_FRect(transform.location.x,transform.location.y,collision_box->w,collision_box->h));
// }

bool Actor::IsActive() const
{
	return is_active;
}

bool Actor::IsVisible() const
{
	return !hidden_in_game;
}

void Actor::AddToWorld(GCPtr<Actor> a) const
{
	game_world->AddToWorld(a);
}
