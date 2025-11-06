#include "../../../Classes/Component/ActorComponent/ActorComponent.hpp"

#include "Classes/Actor.hpp"

void ActorComponent::Construct()
{
    Object::Construct();

}

void ActorComponent::SetOwnerActor(Actor *actor)
{
    outer = actor;
}
