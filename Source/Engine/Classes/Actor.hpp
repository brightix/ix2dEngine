#pragma once
#include "Object.hpp"
#include "Types/Transform.hpp"
#include "Classes/Component/ActorComponent/ActorComponent.hpp"
#include "Component/SenceComponent/SceneComponent.hpp"

struct RenderData;
enum class ActorMobility;
class ActorComponent;
class GameWorld;

class Actor : public Object
{
    //Attribution
	//GCPtr<StaticTexture> collision_box;
//质心就是旋转中心

	//Component
    bool isShowInGame;	//render
	bool is_active;		//tick
	bool hidden_in_game;//run render
	bool is_begin_event_handled;
    Transform transform;

    //每个actor内部有个计时器组件，用来定时处理事件


protected:
	FRect actor_boundary;
	//关卡 负责管理生命周期
	GameWorld* game_world;
	GCPtr<SceneComponent> Root;
	std::vector<SceneComponent*> scene_components;
	std::unordered_map<std::string,GCPtr<ActorComponent>> actor_components;
	//可移动性
public:
	bool is_pre_kill = false; // for render thread 防止悬空指正延迟删除
	bool open_physics;
public:
    //Actor();
	explicit Actor(const Transform &tf);

	void Construct() override;
	void RegisterDispatchers() override;
	void RegisterEvents() override;
	virtual void EventBegin();

	virtual void PrePhysicsTick(double delta_time){}

	virtual void Tick(const double delta_time);
	virtual void PostPhysicsTick(double delta_time){}
	virtual void RootComponentTick(double delta_time);
    virtual void EventEnd(){}

	void HandleComponentPhysics(FPoint application_point, Vec2<float> force);

	void DestroyActor();
    //attribution
	void SetHiddenInGame(bool new_hidden_in_game);
	// void SetMobility(ActorMobility target_mobility);
	// ActorMobility GetMobility() const;

	void SetActorName(const std::string& new_name);


//变换
	[[nodiscard]] Transform GetWorldTransform() const;
    void SetActorTransform(Transform trans);
    void AddActorTransform(Transform trans);
//位置
	void AddActorLocation(const Vec2<float>& added_location) const;
	void SetActorLocation(const Vec2<float>& new_location) const;
	Vec2<float> GetActorWorldLocation() const;
    Vec2<float> GetActorRelativeLocation();
//旋转

	void AddActorWorldLocation(Vec2<float> dis) const;

//修正
	Vec2<float> ConvertLocationFromPivot(Vec2<float> display_corner);
	SceneComponent* GetRoot() const;





	//组件
	void AddActorComponent(const std::string& component_name,ActorComponent* component);

	template<typename T>
	T* GetComponent(const std::string& component_name)
	{
		return Cast<T>(actor_components[component_name]);
	}
	void AddSceneComponent(SceneComponent* sc);

    SceneComponent *GetSceneComponent(const std::string &component_name) const;
	ActorComponent* GetActorComponent(const std::string &component_name) const;
	void SetRoot(SceneComponent* new_root);

//Sys
	virtual void RenderOnScreen();

    void ForRenderOrder(std::vector<RenderData>& data) const;

	bool IsVisible() const;
};

template<typename T>
std::ostream& operator<<(std::ostream & os, Vec2<T> v)
{
	os << "x = " << v.x << " y = " << v.y;
	//os << "x = " + to_string(v.x) + "y = " + to_string(v.y);
	return os;
}