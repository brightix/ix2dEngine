#include "Classes/ActorComponent/ActorComponent.hpp"

ActorComponent::ActorComponent(): owner(nullptr) {}
ActorComponent::ActorComponent(Actor *owner): owner(owner) {}

void ActorComponent::Construct()
{
    Object::Construct();
}
