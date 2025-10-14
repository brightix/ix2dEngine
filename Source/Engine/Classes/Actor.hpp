#pragma once
#include <SDL3/SDL.h>

#include "Object.hpp"
#include "../Structure/Texture.hpp"
#include "Types/Location.hpp"
#include "Types/Transform.hpp"

enum class ActorMobility;
class ActorComponent;
class GameWorld;

class Actor : public Object
{
    //Attribution
	GCPtr<StaticTexture> collision_box;


	//Component
    bool isShowInGame;
	bool is_active;
	bool hidden_in_game;
//每个actor内部有个计时器组件，用来定时处理事件
	ActorMobility mobility;


protected:

	//关卡 负责管理生命周期
	//GameWorld* game_world;

	Transform transform;

	SDL_Renderer* renderer;
	SDL_Window* window;
	GCPtr<GameWorld> game_world;
	std::unordered_map<std::string,GCPtr<ActorComponent>> components;
public:
	bool is_pre_kill = false; // for render thread 防止悬空指正延迟删除
public:

    Actor();
	explicit Actor(const Transform &tf);
	~Actor() override;

	void Construct() override;
    virtual void EventBegin();
	virtual void PrePhysicsTick(double delta_time){}

	virtual void Tick(double delta_time);
	virtual void PostPhysicsTick(double delta_time){}
	virtual void ActorComponentTick(double delta_time);
    virtual void EventEnd(){}


	void DestroyActor();
    //attribution

	void SetMobility(ActorMobility target_mobility);
	//Add
	void AddWorldLocation(Vec2d<float> dis);

    //Get
    Location GetWorldLocation() const;
    Vec2d<float> GetRelativeLocation();


	Transform GetWorldTransform() const;


	template<typename T>
	GCPtr<T> SpawnActorFromSelf(T* actor)
	{
		auto i = GCPtr<T>(actor, this);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		AddToWorld(a);
		this->dispatcher_system.BindEventTo("EventBegin",a,Event("EventBegin",[a](TEventParams) {
			a->EventBegin();
		}));
		return i;
	}
//Sys
	virtual void RenderOnScreen();

    void RenderCollisionBox() const;

    bool IsActive() const;
	bool IsVisible() const;

private:
	void AddToWorld(Actor* a) const;
};

template<typename T>
std::ostream& operator<<(std::ostream & os, Vec2d<T> v)
{
	os << "x = " << v.x << " y = " << v.y;
	//os << "x = " + to_string(v.x) + "y = " + to_string(v.y);
	return os;
}