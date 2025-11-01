#pragma once
#include <SDL3/SDL_rect.h>
#include <format>
#include "Classes/Component/Component.hpp"
#include "Classes/Core/SPhysics/MovableActorUtility.hpp"
#include "Types/Transform.hpp"
#include "Types/Enums/ActorVisibility.hpp"
#include "Types/Enums/LayerHierachy.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

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
	//std::unordered_map<std::type_index,std::vector<GCPtr<SceneComponent>>> mounted_components_by_type;
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
		SceneComponent* component = Cast<SceneComponent>(obj,__func__);
#if DEBUG == 1
		if (!owner)
		{
			std::cout << "有野组件" << std::endl;
		}
#endif
		component->parent_component = this;
		component->SetOwner(owner);
		GCPtr<T> gc_component = NewObject(obj);

		//这里的命名是初始类名+id
		mounted_components.emplace(gc_component->GetComponentName(),gc_component);
		//挂载组件需要已到父组件变换下
		component->SetComponentWorldLocation(transform.location);
		return gc_component;
	}

	void RemoveSceneComponentFromParent() const;
	void RemoveSceneComponent(const std::string& component_name);


	//Danger performance 递归找节点
	GCWeakPtr<SceneComponent> GetSceneComponentByName(const std::string& searched_component_name);
	//单层找节点
	// template<typename T>
	// GCWeakPtr<T> GetSceneComponentByName_SingleLayer(const std::string& searched_component_name)
	// {
	// 	auto it = mounted_components_by_type.find(std::type_index(typeid(T)));
	// 	if (it == mounted_components_by_type.end())
	// 	{
	// 		return nullptr;
	// 	}
	// 	auto components = it->second;
	// 	for (auto& component : components)
	// 	{
	// 		if (component->GetComponentName() == searched_component_name)
	// 		{
	// 			return component;
	// 		}
	// 	}
	// 	return nullptr;
	// }

	//单层
	// template<typename T>
	// GCWeakPtr<T> GetSceneComponent(const std::string& component_name = std::string())
	// {
	// 	auto it = mounted_components_by_type.find(std::type_index(typeid(T)));
	// 	if (it == mounted_components_by_type.end())
	// 	{
	// 		Log(std::format("没找到类型为 {} 的子组件",typeid(T).name()));
	// 		return {};
	// 	}
	// 	if (!component_name.empty())
	// 	{
	// 		auto components = it->second;
	// 		for (auto& component : components)
	// 		{
	// 			if (component->name == component_name)
	// 			{
	// 				return component;
	// 			}
	// 		}
	// 		Log(std::format("没找到类型为 {},名为 {} 的子组件",typeid(T).name(), component_name));
	// 		return {};
	// 	}
	// 	return it->second[0];
	// }

	//typeid 添加子组件
	// template<typename T>
	// GCWeakPtr<T> MountedSceneComponent(std::string component_name = std::string())
	// {
	// 	ix::IsChild<SceneComponent,T>();
	//
	// 	auto new_component = NewObject<T>(new T);
	// 	if (!component_name.empty())
	// 	{
	// 		if (GetSceneComponentByName_SingleLayer<T>(component_name))
	// 		{
	// 			//重名了就换为 类名+id
	// 			std::string s = new_component->GetClassName() + std::to_string(new_component.id);
	// 			Log(std::format("命名 {} 已被占用,改名为 {}", component_name,s));
	// 			component_name = std::move(s);
	// 		}
	// 	}
	// 	new_component->SetComponentName(component_name);
	//
	// 	mounted_components_by_type[std::type_index(typeid(T))].emplace_back(new_component);
	//
	// 	return new_component;
	// }

    void ForRender();
	void ForRenderData(std::vector<RenderData>& data);
    virtual void OfferRenderData(std::vector<RenderData>& data);

	bool IsSceneComponentOpenedPhysics() const;

	SDL_FRect GetComponentRenderRect() const;


	/// DebugOnly
public:

	virtual void Debug_RenderOutline(std::vector<RenderData>& data);


	/**
	 * 设置组件的命名
	 * @param new_name
	 * @return new_name 未使用则改名成功，否则不做任何操作
	 */
	bool SetName(const std::string& new_name) override;

	/**
	 * 在子组件触发改名时调用父组件修改挂载表的关系
	 * @param component_name 原命名
	 * @param new_name 姓命名
	 * @return
	 */
	bool OnMountedComponentNameChanged(const std::string& component_name, const std::string& new_name);




	Vec2<float> GetComponentSize();


	Transform GetComponentTransform();
	void SetComponentTransform(Transform new_transform);

	void AddComponentWorldLocation(const Vec2<float>& added_loc);
	void SetComponentWorldLocation(const Location& new_loc);
	Location GetComponentWorldLocation();
	void SetComponentWorldRotation(const Rotation& rotation);
	void AddComponentWorldRotation(const Rotation& rotation);
};

