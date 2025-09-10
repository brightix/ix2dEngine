#pragma once
#include <SDL3/SDL.h>

#include "Object.h"
#include "Types/Transform.hpp"

class Actor : public Object
{
    //Attribution
    Transform transform;

    bool isShowInGame;
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
public:
    Actor();
	explicit Actor(Transform tf);
	~Actor() override;

    virtual void EventBegin(){}
    virtual void Tick(double deltaTime){}
    virtual void EventEnd(){}
    //attribution


    //Get
    Vec2d<float> GetWorldLocation();
    Vec2d<float> GetRelativeLocation();
	template<typename T>
	GCPtr<T> SpawnActorFromSelf(T* actor)
	{
		return GCPtr<T>(actor, this);
	}

private:
    void Construct();
};