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
    int w;
    int h;

	/**
     * 渲染枢轴
     */
    Vec2<float> pivot;
	GCPtr<SPhysicsBaseUtility> physics_body;
	std::unordered_map<std::string, GCPtr<SceneComponent>> mounted_components;
	ComponentVisibility visibility = ComponentVisibility::Visible;
	bool open_physics = true;
public:
	LayerHierarchy layer;
    SceneComponent();
    explicit SceneComponent(const Transform& trans);

	void Construct() override;


	Transform GetComponentTransform();
	void AddComponentWorldLocation(const Vec2<float>& added_loc);

	void SetComponentWorldLocation(const Location& new_loc);


	//可视性
	void SetVisibility(ComponentVisibility new_visibility);
	ComponentVisibility GetVisibility();
	bool IsVisible() const;

	//单线程用
    virtual void ComponentRender();
	virtual void SceneComponentTick(double delta_time);
	//挂载子场景组件
	void MountedComponent(GCPtr<SceneComponent> child_component);



	//Danger performance 递归找节点
	GCPtr<SceneComponent> GetSceneComponentByName(const std::string& searched_component_name)
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

	void SetComponentWorldRotation(const Rotation& rotation);

	void AddComponentWorldRotation(const Rotation& rotation);
};
