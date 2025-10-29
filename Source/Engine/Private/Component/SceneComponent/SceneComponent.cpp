#include "Classes/Component/SenceComponent/SceneComponent.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Core/GameEngine.hpp"
#include "Types/RenderData.hpp"
#include "Utilities/FuncLib/Deleter.hpp"

SceneComponent::SceneComponent() : SceneComponent(Transform{}){ }
SceneComponent::SceneComponent(const Transform& trans) : w(0), h(0) { }

void SceneComponent::Construct()
{
	Component::Construct();
	dispatcher_system.AddEventDispatcher("OnSceneComponentTeleport");
}

void SceneComponent::NativeSceneComponentEventBegin()
{
	ComponentEventBegin();
	for (const auto& sc : mounted_components | std::views::values)
	{
		sc->NativeSceneComponentEventBegin();
	}
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
	for (auto& it : mounted_components | std::views::values)
	{
		it->SceneComponentTick(delta_time);
	}
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

void SceneComponent::OfferRenderData(std::vector<RenderData>& data)
{}

bool SceneComponent::IsSceneComponentOpenedPhysics() const { return open_physics; }

SDL_FRect SceneComponent::GetComponentRenderRect() const
{
	return SDL_FRect(transform.location.x - w * pivot.x, transform.location.y - h * pivot.y, w * transform.scaling.horizontal, h * transform.scaling.vertical);
}

Vec2<float> SceneComponent::GetComponentVisibleScale() const
{
	return {w * transform.scaling.horizontal, h * transform.scaling.vertical};
}

Vec2<float> SceneComponent::GetComponentRenderLocation() const
{
	return {transform.location.x - w * pivot.x, transform.location.y - h * pivot.y};
}

void SceneComponent::Debug_RenderOutline(std::vector<RenderData>& data)
{
	//单线程
	auto t = TTexture(RendererCenter::CreateOutLineTexture(Vec2<float>(w,h)));
	data.emplace_back(RenderData{t ,transform,{},SDL_FRect(transform.location.x,transform.location.y,w,h)});
}


bool SceneComponent::SetComponentName(const std::string& new_name)
{
	if (parent_component && parent_component->OnMountedComponentNameChanged(name,new_name))
	{
		name = new_name;
		return true;
	}
	return false;
}

bool SceneComponent::OnMountedComponentNameChanged(const std::string& component_name, const std::string& new_name)
{

	auto it = mounted_components.find(new_name);
	if (it != mounted_components.end())
	{
		Log("场景组件命名已使用，驳回改名请求");
		return false;
	}
	mounted_components[new_name] = std::move(mounted_components[component_name]);
	mounted_components.erase(component_name);
	return true;
}


Transform SceneComponent::GetComponentTransform()
{
	return transform;
}

//变换
void SceneComponent::SetComponentTransform(Transform new_transform)
{
	new_transform.location += {relative_location.x,relative_location.y};
	transform = new_transform;
	// if (open_physics)
	// {
	// 	physics_body->SetComponentTransform(transform);
	// }
	for (const auto& val : mounted_components | std::views::values)
	{
		val->SetComponentTransform(transform);
	}
}

//位置
void SceneComponent::AddComponentWorldLocation(const Vec2<float>& added_loc)
{
	transform.location += added_loc;
	for (const auto& val : mounted_components | std::views::values)
	{
		val->AddComponentWorldLocation(added_loc);
	}
}
void SceneComponent::SetComponentWorldLocation(const Location& new_loc)
{
	//传递一个新位置，所有的子组件都需要以父位置为基准，偏移 相对量 位置
	Location absolute_location = new_loc + relative_location;
	transform.location = absolute_location;

	for (const auto& val : mounted_components | std::views::values)
	{
		val->SetComponentWorldLocation(absolute_location);
	}
	dispatcher_system.CallDispatcher("OnSceneComponentTeleport");
}

Location SceneComponent::GetComponentWorldLocation()
{
	return transform.location;
}


//旋转
// TODO 可能有问题
void SceneComponent::SetComponentWorldRotation(const Rotation& rotation)
{
	//点绕点旋转
	if (rotation.Point)
	{
		// if (open_physics)
		// {
		// 	physics_body->SetBodyWorldRotation(rotation);
		// }
		transform.location.RotateByAngle(rotation.Angle,{rotation.Point->x,rotation.Point->y});

		//TODO 自转
	}
	transform.rotation.Angle = rotation.Angle;
	for (const auto& val : mounted_components | std::views::values)
	{
		val->SetComponentWorldRotation(rotation);
	}
}

void SceneComponent::AddComponentWorldRotation(const Rotation& rotation)
{
	SetComponentWorldRotation({Rotation::Normalize(rotation.Angle),rotation.Point});
}
