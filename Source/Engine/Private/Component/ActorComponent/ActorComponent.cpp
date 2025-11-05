#include "../../../Classes/Component/ActorComponent/ActorComponent.hpp"

void ActorComponent::Construct()
{
    Object::Construct();

}

void ActorComponent::SetOwnerActor(Actor *actor)
{
    owner = actor;
}
