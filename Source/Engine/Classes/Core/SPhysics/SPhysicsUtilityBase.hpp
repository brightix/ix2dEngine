#pragma once
#include <string>

#include "Classes/Object.hpp"
#include "Types/FRect.hpp"
#include "Types/Location.hpp"
#include "Types/Rotation.hpp"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"

class SPhysicsBaseUtility;
class SceneComponent;

enum class PhysicsType
{
    Static,
    RigidBody,
    Movable
};

struct PhysicsState
{
	//用于表示物理包属于谁
	GCWeakPtr<SPhysicsBaseUtility> physics_object;

	Vec2<float> position;

	Vec2<float> added_force;
	Vec2<float> velocity;
	float quality;
	float force_attenuation;
};

class SPhysicsBaseUtility : public Object
{
public:
	bool simulation_physics;
	bool is_subscribe_collision;
	~SPhysicsBaseUtility() override;

//物理的位置跟随父场景组件
	Vec2<float> velocity;		//当前移动向量
	float quality = 2.f;		//物体质量
	float force_attenuation = 1.0f;		//力衰减
	float bounciness = 0.5f;

	PhysicsType type;
	SceneComponent* collision_owner;

	//外力
	Vec2<float> added_force;	//冲量

	//结果
	Location after_location;
	Rotation after_rotation;


	// Hook
	std::function<void()> physics_callback;
	void SetPhysicsCallback(const std::function<void()>& physics_callback_);

    SPhysicsBaseUtility();
	void Construct() override;



    virtual PhysicsType GetPhysicsType(){ return type; }
    [[nodiscard]] FRect GetCollisionBox() const;

	void SetSimulationPhysics(bool is_active, PhysicsType new_type = PhysicsType::Static);
	void SetPhysicsType(PhysicsType new_type);
    virtual void HandleVelocity(double delta_time);



//位移需要变换位置
	void AddBodyWorldLocation(Vec2<float> v);




    virtual void Init();
    // void SetIsSimulatedPhysics(bool value);
	void SetOwner(SceneComponent* new_owner);
	void SetSubscribeCollision(bool is_subscribe);
//同步
	void SynchronizationTransform();


	// void SetBodyBox(Vec2<float> size);
	void SetBodyTransform(Transform transform);

	void SetBodyWorldLocation(const Location& location);
	void SetBodyWorldRotation(const Rotation& rotation);
};