#include "Classes/Component/SenceComponent/SceneComponent.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Core/GameEngine.hpp"

SceneComponent::SceneComponent() : SceneComponent(Transform{}){ }
SceneComponent::SceneComponent(const Transform& trans) : w(0), h(0) { }
Transform SceneComponent::GetComponentTransform()
{
	return transform;
}

void SceneComponent::AddComponentWorldLocation(Vec2<float> v)
{
	transform.location += v;
	for (auto& component : child_components)
	{
		component.second->AddComponentWorldLocation(v);
	}
}

void SceneComponent::ComponentRender() {}
void SceneComponent::AddToRoot(const std::string& component_name,GCPtr<SceneComponent> child_component)
{
	child_components.emplace(component_name,child_component);
}

void SceneComponent::ForRender()
{
	ComponentRender();
	for (auto& component : child_components)
	{
		component.second->ForRender();
	}
}
