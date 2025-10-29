#include "Classes/Actor.hpp"

#include "Classes/Component/ActorComponent/ActorComponent.hpp"
#include "../Classes/Component/SenceComponent/RootComponent.h"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Enum/ActorEnum.hpp"

Actor::Actor() : Actor(Transform()){}
Actor::Actor(const Transform &tf) : isShowInGame(true), is_active(true), hidden_in_game(false),
                                    is_begin_event_handled(false),
                                    window(nullptr), transform(tf),
                                    mobility(ActorMobility::Static), open_physics(false)
{
	NAME;
}

void Actor::Construct()
{
	//事件

	//场景默认根组件
	Root = NewObject<SceneComponent>(new RootComponent(Transform{{0,0}}));
	// TODO处理事件回调时机，先绑定事件还是先设置位置

	Root->MountedComponent(new StaticTexture())->name = "default_texture";
	SetActorTransform(transform);
}

void Actor::EventBegin()
{
	game_world = World();
	is_begin_event_handled = true;
	//场景根
	Root->NativeSceneComponentEventBegin();
	//逻辑组件
	for (auto& ac : actor_components | std::views::values)
	{
		ac->ComponentEventBegin();
	}
	//game_world = GameEngine::Instance().GetGameWorld().Get();
}

//所有重写tick都在PreTick内
void Actor::Tick(double delta_time) {}

void Actor::RootComponentTick(const double delta_time)
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


GCWeakPtr<SceneComponent> Actor::GetSceneComponent(const std::string& component_name) const
{
	return Root->GetSceneComponentByName(component_name);
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


bool Actor::IsVisible() const
{
	return !hidden_in_game;
}
//=====================================================
// 🧠 Transform 控制
//=====================================================
void Actor::SetActorTransform(Transform trans)
{
	Root->SetComponentWorldLocation(trans.location);
	Root->SetComponentWorldRotation(trans.rotation);
}

void Actor::AddActorTransform(Transform trans)
{
	Root->AddComponentWorldLocation({trans.location.x,trans.location.y});
	Root->AddComponentWorldRotation(trans.rotation);
}

void Actor::AddActorWorldLocation(Vec2<float> dis) const
{
	Root->AddComponentWorldLocation(dis);
}
//=====================================================
// Get 属性
//=====================================================
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