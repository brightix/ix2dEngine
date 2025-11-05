#include "Classes/Component/SenceComponent/SceneComponent.hpp"

#include <SDL3/SDL_render.h>

#include "Classes/Actor.hpp"
#include "Classes/Component/SenceComponent/CollisionBox.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Types/RenderData.hpp"

SceneComponent::SceneComponent() : SceneComponent(Transform{}){ }
SceneComponent::SceneComponent(const Transform& trans) : w(0), h(0), parent_component(nullptr), layer() { }

void SceneComponent::Construct()
{
	Component::Construct();
	dispatcher_system.AddEventDispatcher("OnComponentLocationChanged");
	dispatcher_system.AddEventDispatcher("OnComponentRotationChanged");
	dispatcher_system.AddEventDispatcher("OnComponentScalingChanged");
	//dispatcher_system.AddEventDispatcher("OnSceneComponentTeleport");
}

void SceneComponent::NativeSceneComponentEventBegin()
{
	ComponentEventBegin();
	for (const auto& sc : mounted_components | std::views::values)
	{
		sc->NativeSceneComponentEventBegin();
	}
}

void SceneComponent::SetOwnerActor(Actor *actor)
{
#if DEBUG
	if (!actor)
	{
		Log(name + "被空Actor拥有");
	}
	if (!owner && actor)
	{
		Log(name + "已经重新被" + actor->name + "拥有");
	}

#endif
	owner = actor;
	for (const auto& component : mounted_components | std::views::values)
	{
		component->SetOwnerActor(actor);
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

void SceneComponent::Clear()
{


}

void SceneComponent::NativeClear()
{
	Clear();
	for (auto& component : mounted_components | std::views::values)
	{
		component->NativeClear();
	}
}

//TODO 可能没什么用
void SceneComponent::Destroy()
{
	is_pending_kill = true;
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

void SceneComponent::RemoveSceneComponentFromParent() const
{
	parent_component->RemoveSceneComponent(name);
}

void SceneComponent::RemoveSceneComponent(const std::string& component_name)
{
	mounted_components.erase(component_name);
}

GCPtr<SceneComponent> SceneComponent::GetSceneComponentByName(const std::string& searched_component_name)
{
	for (const auto& component : mounted_components | std::views::values)
	{
		if (component->name == searched_component_name)
		{
			return component;
		}
		if (auto ret = component->GetSceneComponentByName(searched_component_name); ret.IsValid())
		{
			return ret;
		}
	}
	return {};
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

void SceneComponent::OfferRenderData(std::vector<RenderData>& data){}

bool SceneComponent::IsSceneComponentOpenedPhysics() const { return open_physics; }

SDL_FRect SceneComponent::GetComponentRenderRect() const
{
	return SDL_FRect(world_transform.location.x - w * pivot.x,
						world_transform.location.y - h * pivot.y,
						w * world_transform.scaling.horizontal,
						h * world_transform.scaling.vertical);
}
//
// Vec2<float> SceneComponent::GetComponentVisibleScale() const
// {
// 	return {w * transform.scaling.horizontal, h * transform.scaling.vertical};
// }
//
// Vec2<float> SceneComponent::GetComponentRenderLocation() const
// {
// 	return {transform.location.x - w * pivot.x, transform.location.y - h * pivot.y};
// }

void SceneComponent::Debug_RenderOutline(std::vector<RenderData>& data)
{
	//单线程
	auto t = Create_OutLineTexture_S(Vec2<float>(w,h));
	data.emplace_back(RenderData{t ,world_transform,{},SDL_FRect(world_transform.location.x,world_transform.location.y,w,h)});
}


bool SceneComponent::SetComponentName(const std::string& new_name)
{
	if (!parent_component || !parent_component->OnMountedComponentNameChanged(name,new_name))
	{
		LogWithLevel("组件名已被占用", FatalError);
		return false;
	}
	name = new_name;
	return true;
}

void SceneComponent::SetRenderLayer(LayerHierarchy layer_id)
{
	layer = layer_id;
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

Vec2<float> SceneComponent::GetComponentSize()
{
	return {w, h};
}


Transform SceneComponent::GetComponentTransform()
{
	return world_transform;
}

//变换
void SceneComponent::SetComponentTransform(Transform new_transform)
{
	new_transform.location += relative_location;
	world_transform = new_transform;
	// if (open_physics)
	// {
	// 	physics_body->SetComponentTransform(transform);
	// }
	for (const auto& val : mounted_components | std::views::values)
	{
		val->SetComponentTransform(world_transform);
	}
}

//位置
void SceneComponent::AddComponentWorldLocation(const Vec2<float>& added_loc)
{
	world_transform.location += added_loc;
	for (const auto& val : mounted_components | std::views::values)
	{
		val->AddComponentWorldLocation(added_loc);
	}
}
void SceneComponent::SetComponentWorldLocation(const Vec2<float>& new_loc)
{
	if (world_transform.location == new_loc)
	{
		//如果位置无变化就不做处理，防止循环依赖
		return ;
	}
	world_transform.location = new_loc;
	if (parent_component)
	{
		relative_location = world_transform.location - parent_component->GetComponentWorldLocation();
	}
	for (const auto& val : mounted_components | std::views::values)
	{
		//手动获取组件的  相对位置，然后告诉他应该的  绝对位置
		const Vec2<float> absolute_location = new_loc + val->GetComponentRelativeLocation();
		val->SetComponentWorldLocation(absolute_location);
	}
	dispatcher_system.CallDispatcher("OnComponentLocationChanged");
}

Vec2<float> SceneComponent::GetComponentWorldLocation()
{
	return world_transform.location;
}

Vec2<float> SceneComponent::GetComponentRelativeLocation()
{
	return relative_location;
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
		world_transform.location.RotateByAngle(rotation.Angle,{rotation.Point->x,rotation.Point->y});

		//TODO 自转
	}
	world_transform.rotation.Angle = rotation.Angle;
	for (const auto& val : mounted_components | std::views::values)
	{
		val->SetComponentWorldRotation(rotation);
	}
}

void SceneComponent::AddComponentWorldRotation(const Rotation& rotation)
{
	SetComponentWorldRotation({Rotation::Normalize(rotation.Angle),rotation.Point});
}

Rotation SceneComponent::GetComponentWorldRotation() const
{
	return world_transform.rotation;
}

Rotation SceneComponent::GetComponentRelativeRotation()
{
	return relative_rotation;
}


bool SceneComponent::Replace(SceneComponent *old_component, SceneComponent *new_component)
{
	// if (old_component->parent_component && !force)
	// {
	//
	// 	Log("父组件不为 Root 并且行为是 非强制");
	// 	return false;
	// }
	const auto parent_actor = old_component->GetOwner();
	const auto parent_component = old_component->outer;
	//old_component->Destroy();
	//传递挂载组件
	for (auto& component : old_component->mounted_components | std::views::values)
	{
		new_component->MountedComponent(component.Get());
	}

	if (!outer)
	{
		if (!parent_actor)
		{
			Log("组件既不属于Actor，也没有挂载在父组件上");
			return false;
		}
		parent_actor->SetRoot(new_component);
	}
	else
	{
		GCUnLink(new_component->outer,new_component);
		new_component->GCUnlink_self();
		new_component->outer = outer;
	}
	return true;
}