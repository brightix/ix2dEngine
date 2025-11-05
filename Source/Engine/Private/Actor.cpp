#include "Classes/Actor.hpp"

#include "Classes/Component/ActorComponent/ActorComponent.hpp"
#include "../Classes/Component/SenceComponent/RootComponent.h"
#include "Classes/Component/SenceComponent/SceneTextBlock.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Enum/ActorEnum.hpp"

Actor::Actor() : Actor(Transform()){}
Actor::Actor(const Transform &tf) : isShowInGame(true), is_active(true), hidden_in_game(false),
                                    is_begin_event_handled(false),
                                    window(nullptr), transform(tf),
                                    open_physics(false)
{
	CNAME;
}

void Actor::Construct()
{
	//事件
	//event_system.AddEvent("");
	dispatcher_system.AddEventDispatcher("OnMobilityChanged");
	//场景默认根组件
	Root = NewObject<RootComponent>(this);
	Root->SetOwnerActor(this);

	// TODO处理事件回调时机，先绑定事件还是先设置位置


	auto tex = Root->MountedComponent(NewObject<StaticTexture>());
	tex->SetComponentName("default_texture");
	tex->SetNewTexture(Create_FilledTexture_S({100,100}));


	auto NameBlock = NewObject<SceneTextBlock>();
	NameBlock->SetText(name);
	Root->MountedComponent(NameBlock);
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


void Actor::HandleComponentPhysics(FPoint application_point, Vec2<float> force)
{

}

void Actor::DestroyActor()
{
	//单线程
	// is_pending_kill = true;
	// is_active = false;

	//三缓冲
	is_pre_kill = true;
	is_active = false;
	GCUnlink_self();
}

void Actor::SetHiddenInGame(bool new_hidden_in_game)
{
	hidden_in_game = new_hidden_in_game;
}

// void Actor::SetMobility(const ActorMobility target_mobility)
// {
// 	mobility = target_mobility;
// 	dispatcher_system.CallDispatcher("OnMobilityChanged");
// }
//
// ActorMobility Actor::GetMobility() const
// {
// 	return mobility;
// }

void Actor::SetActorName(const std::string &new_name)
{
	name = new_name;
}


GCPtr<SceneComponent> Actor::GetSceneComponent(const std::string& component_name) const
{
	return Root->GetSceneComponentByName(component_name);
}

void Actor::SetRoot(SceneComponent *new_root)
{

	Root = new_root;
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
Vec2<float> Actor::GetWorldLocation() const
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