#pragma once
#include <SDL3/SDL.h>

#include "Object.h"
#include "System/Texture.hpp"
#include "Types/Transform.hpp"

class GameWorld;

class Actor : public Object
{
    //Attribution
    Transform transform;
	GCPtr<StaticTexture> static_texture;

    bool isShowInGame;

	//关卡 负责管理生命周期
	GCPtr<GameWorld> game_world;
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
public:
    Actor();
	explicit Actor(Transform tf);
	~Actor() override;

	void Construct() override;
    virtual void EventBegin();
    virtual void Tick(double deltaTime);
    virtual void EventEnd(){}
    //attribution

	//Add
	void AddWorldLocation(Vec2d<float> dis);

    //Get
    Vec2d<float> GetWorldLocation();
    Vec2d<float> GetRelativeLocation();

	template<typename T>
	GCPtr<T> SpawnActorFromSelf(T* actor)
	{
		auto i = GCPtr<T>(actor, this);
		auto a = static_cast<Actor*>(i.Get());
		a->Construct();
		AddToWorld(a);
		return i;
	}
//Sys
	virtual void RenderOnScreen();
private:
	void AddToWorld(Actor* a);
};

template<typename T>
std::ostream& operator<<(std::ostream & os, Vec2d<T> v)
{
	os << "x = " << v.x << " y = " << v.y;
	//os << "x = " + to_string(v.x) + "y = " + to_string(v.y);
	return os;
}