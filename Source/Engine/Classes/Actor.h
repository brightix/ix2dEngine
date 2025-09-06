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
    Actor(Transform tf);
    ~Actor() override;
public:

    virtual void EventBegin(){}
    virtual void Tick(double deltaTime){}
    virtual void EventEnd(){}
    //attribution


    //Get
    Vec2d<float> GetWorldLocation();
    Vec2d<float> GetRelativeLocation();
private:
    void Construct();
};