#pragma once
#include <SDL3/SDL_rect.h>

#include "Classes/Component/Component.hpp"
#include "Classes/Core/SPhysics/MovableActorUtility.hpp"
#include "Types/Transform.hpp"
#include "Types/Enums/ActorVisibility.hpp"
#include "Types/Enums/LayerHierachy.hpp"

struct RenderData;

class SceneComponent : public Component
{
protected:

	/**
     * 逻辑变换
     */
    Transform transform;
	Location relative_location;
	Rotation relative_rotation;
	float w;
	float h;

	/**
     * 渲染枢轴
     */
    Vec2<float> pivot;
	std::unordered_map<std::string, GCPtr<SceneComponent>> mounted_components;
	SceneComponent* parent_component;
	ComponentVisibility visibility = ComponentVisibility::Visible;
	bool open_physics = false;
public:
	LayerHierarchy layer;
    SceneComponent();
    explicit SceneComponent(const Transform& trans);

	void Construct() override;

	void NativeSceneComponentEventBegin();


	//可视性
	void SetVisibility(ComponentVisibility new_visibility);
	ComponentVisibility GetVisibility();
	bool IsVisible() const;

	//单线程用
    virtual void ComponentRender();
	virtual void SceneComponentTick(double delta_time);
	//挂载子场景组件
	template<typename T>
	GCWeakPtr<T> MountedComponent(T* obj)
	{
		static_assert(std::is_base_of_v<SceneComponent, T>, "T must derive from Component");

		SceneComponent* component = dynamic_cast<SceneComponent*>(obj);
		assert(component && "Object must derive from Component");

		component->parent_component = this;
		GCPtr<T> gc_component = NewObject(obj);

		mounted_components.emplace(gc_component->GetComponentName(),gc_component);
		//挂载组件需要已到父组件变换下
		component->SetComponentWorldLocation(transform.location);
		return gc_component;
	}



	//Danger performance 递归找节点
	GCWeakPtr<SceneComponent> GetSceneComponentByName(const std::string& searched_component_name)
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
    void ForRender();
	void ForRenderData(std::vector<RenderData>& data);
    virtual void OfferRenderData(std::vector<RenderData>& data);

	bool IsSceneComponentOpenedPhysics() const;


	SDL_FRect GetComponentRenderRect() const;
	/**
	 *
	 * @return 返回组件的渲染缩放
	 */
	Vec2<float> GetComponentVisibleScale() const;
    /**
	 *
	 * @return 返回Actor的渲染位置
	 */
	Vec2<float> GetComponentRenderLocation() const;
/// DebugOnly
public:

	virtual void Debug_RenderOutline(std::vector<RenderData>& data);


	/**
	 * 设置组件的命名
	 * @param new_name
	 * @return new_name 未使用则改名成功，否则不做任何操作
	 */
	bool SetComponentName(const std::string& new_name) override;

	/**
	 * 在子组件触发改名时调用父组件修改挂载表的关系
	 * @param component_name 原命名
	 * @param new_name 姓命名
	 * @return
	 */
	bool OnMountedComponentNameChanged(const std::string& component_name, const std::string& new_name);


	Transform GetComponentTransform();
	void SetComponentTransform(Transform new_transform);

	void AddComponentWorldLocation(const Vec2<float>& added_loc);
	void SetComponentWorldLocation(const Location& new_loc);
	Location GetComponentWorldLocation();
	void SetComponentWorldRotation(const Rotation& rotation);
	void AddComponentWorldRotation(const Rotation& rotation);
};

