#include <Actor.h>
#include <GameInstance.hpp>
void Actor::Construct()
{
    renderer = GameInstance::Instance().GetRenderer();
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