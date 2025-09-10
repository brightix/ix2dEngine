#include "Classes/Actor.h"
#include "../System/GameEngine.hpp"

Vec2d<float> Actor::GetWorldLocation()
{
	return {};
}

Vec2d<float> Actor::GetRelativeLocation()
{
	return {};
}

void Actor::Construct()
{
    //renderer = GameEngine::Instance().GetRenderer();
}
Actor::Actor() : Object()
{
    Construct();
}

Actor::Actor(Transform tf)
{
    transform = tf;
    Construct();
}

Actor::~Actor() {}
