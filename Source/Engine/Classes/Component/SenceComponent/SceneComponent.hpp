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
	std::unordered_map<std::string, GCPtr<SceneComponent>> child_components;
public:
    SceneComponent();

    explicit SceneComponent(const Transform& trans);
	Transform GetComponentTransform();
	void AddComponentWorldLocation(Vec2<float> v);
    virtual void ComponentRender();


	//挂载子场景组件
	void AddToRoot(const std::string& component_name, GCPtr<SceneComponent> child_component);
    void ForRender();
};
