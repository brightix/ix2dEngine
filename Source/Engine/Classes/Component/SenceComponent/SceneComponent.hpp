#pragma once
#include "Classes/Component/Component.hpp"
#include "Types/Transform.hpp"

class SceneComponent : public Component
{
protected:
    Transform transform;
    int w;
    int h;
    Vec2<float> pivot;
	std::unordered_map<std::string, GCPtr<SceneComponent>> mounted_components;
public:
    SceneComponent();

    explicit SceneComponent(const Transform& trans);
	Transform GetComponentTransform();
	void AddComponentWorldLocation(Vec2<float> v);
	void SetComponentWorldLocation(const Location& new_loc);
    virtual void ComponentRender();

	virtual void SceneComponentTick(double delta_time);
	//挂载子场景组件
	void MountedComponent(GCPtr<SceneComponent> child_component);


	//Danger performance 递归找节点
	GCPtr<SceneComponent> GetSceneComponentByName(const std::string& searched_component_name)
	{
		for (const auto& component : mounted_components | std::views::values)
		{
			if (component->component_name == searched_component_name)
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
};
