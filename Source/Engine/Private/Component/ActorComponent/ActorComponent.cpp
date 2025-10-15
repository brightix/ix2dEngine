#include "../../../Classes/Component/ActorComponent/ActorComponent.hpp"

ActorComponent::ActorComponent(): owner(nullptr) {}
ActorComponent::ActorComponent(Actor *owner): owner(owner) {}

void ActorComponent::Construct()
{
    Object::Construct();

}

void ActorComponent::SetOwner(Actor* owner_actor)
{
    owner = owner_actor;
}
