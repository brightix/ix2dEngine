#pragma once
#include <SDL3/SDL.h>

#include "Object.hpp"
#include "Types/Location.hpp"
#include "Types/Transform.hpp"
#include "Utilities/FuncLib/StaticCast.hpp"
#include "Classes/Component/ActorComponent/ActorComponent.hpp"

struct RenderData;
class SceneComponent;
enum class ActorMobility;
class ActorComponent;
class GameWorld;

class Actor : public Object
{
    //Attribution
	//GCPtr<StaticTexture> collision_box;
	Transform transform;

	//Component
    bool isShowInGame;
	bool is_active;
	bool hidden_in_game;
	bool is_begin_event_handled;

//每个actor内部有个计时器组件，用来定时处理事件


protected:

	//关卡 负责管理生命周期
	SDL_Window* window;
	GCWeakPtr<GameWorld> game_world;

	GCPtr<SceneComponent> Root;
	std::unordered_map<std::string,GCPtr<ActorComponent>> actor_components;
	//可移动性
	ActorMobility mobility;
public:
	bool is_pre_kill = false; // for render thread 防止悬空指正延迟删除
	bool open_physics;
public:

    Actor();
	explicit Actor(const Transform &tf);

	void Construct() override;
    virtual void EventBegin();
	virtual void PrePhysicsTick(double delta_time){}

	virtual void Tick(double delta_time);
	virtual void PostPhysicsTick(double delta_time){}
	virtual void RootComponentTick(double delta_time);
    virtual void EventEnd(){}


	//void SimulationPhysics();

	void DestroyActor();
    //attribution

	void SetMobility(ActorMobility target_mobility);

    void SetActorTransform(Transform trans);

    void AddActorTransform(Transform trans);

    //Add
	void AddActorWorldLocation(Vec2<float> dis) const;

    //Get
    Location GetWorldLocation() const;
    Vec2<float> GetRelativeLocation();


	Transform GetWorldTransform() const;








	//组件
	template<typename T>
	void AddActorComponent(const std::string& component_name,T* component)
	{
		static_assert(std::is_base_of_v<ActorComponent, T>, "类必须继承自ActorComponent");
		actor_components[component_name] = NewObject<T>(component);
		component->SetOwner(this);
	}

	template<typename T>
	T* GetComponent(const std::string& component_name)
	{
		return Cast<T>(actor_components[component_name].Get());
	}
	GCWeakPtr<SceneComponent> GetSceneComponent(const std::string& component_name) const;

//Sys
	virtual void RenderOnScreen();

    void ForRenderOrder(std::vector<RenderData>& data) const;

	bool IsVisible() const;
};

template<typename T>
std::ostream& operator<<(std::ostream & os, Vec2<T> v)
{
	os << "x = " << v.x << " y = " << v.y;
	//os << "x = " + to_string(v.x) + "y = " + to_string(v.y);
	return os;
}