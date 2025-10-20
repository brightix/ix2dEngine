#include "Classes/Component/SenceComponent/SceneComponent.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Core/GameEngine.hpp"
#include "Types/RenderData.hpp"

SceneComponent::SceneComponent() : SceneComponent(Transform{}){ }
SceneComponent::SceneComponent(const Transform& trans) : w(0), h(0) { }
Transform SceneComponent::GetComponentTransform()
{
	return transform;
}

void SceneComponent::AddComponentWorldLocation(Vec2<float> v)
{
	transform.location += v;
	for (const auto& val : mounted_components | std::views::values)
	{
		val->AddComponentWorldLocation(v);
	}
}

void SceneComponent::SetComponentWorldLocation(const Location& new_loc)
{
	transform.location.x = new_loc.x;
	transform.location.y = new_loc.y;
}

void SceneComponent::SetVisibility(ComponentVisibility new_visibility)
{
	visibility = new_visibility;
}

ComponentVisibility SceneComponent::GetVisibility()
{
	return visibility;
}

bool SceneComponent::IsVisible() const
{
	return visibility == ComponentVisibility::Visible || visibility == ComponentVisibility::OnlyDraw;
}

void SceneComponent::ComponentRender() {}
void SceneComponent::SceneComponentTick(double delta_time)
{
	//先tick自己，后tick挂载的组件
	ComponentTick(delta_time);
	for (auto& it : mounted_components)
	{
		it.second->SceneComponentTick(delta_time);
	}
}
void SceneComponent::MountedComponent(GCPtr<SceneComponent> child_component)
{
	mounted_components.emplace(child_component->GetComponentName(),child_component);
}



//递归调用接口
void SceneComponent::ForRender()
{
	ComponentRender();
	for (const auto& child : mounted_components | std::views::values)
	{
		child->ForRender();
	}
}

void SceneComponent::OfferRenderData(std::vector<RenderData>& data)
{}


//递归调用接口
void SceneComponent::ForRenderData(std::vector<RenderData>& data)
{
	OfferRenderData(data);
	for (const auto& child : mounted_components | std::views::values)
	{
		if (child->IsVisible())
		{
			child->ForRenderData(data);
		}
	}
}
