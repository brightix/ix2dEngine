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

    virtual void EventBegin();
    virtual void Tick(double deltaTime){}
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
		return i;
	}
//Sys
	virtual void RenderOnScreen();
private:
    void Construct() override;
};
