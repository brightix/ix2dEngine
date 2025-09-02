#pragma once
#include <Transform.hpp>
#include <SDL3/SDL.h>
class Object{};

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
public:

    virtual void EventBegin(){}
    virtual void Tick(double delta){}
    virtual void EventEnd(){}
    //attribution


    //Get
    Vec2d<float> GetWorldLocation();
    Vec2d<float> GetRelativeLocation();
private:
    void Construct();
};