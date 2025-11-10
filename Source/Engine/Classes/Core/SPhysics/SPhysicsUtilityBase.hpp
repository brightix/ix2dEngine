#pragma once
#include "Classes/Object.hpp"
#include "Types/FRect.hpp"
#include "Types/Rotation.hpp"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"

class SceneComponent;
class SPhysicsBaseUtility;

enum class PhysicsType
{
    Static,
    RigidBody,
    Movable
};

struct PhysicsState
{
	//用于表示物理包属于谁
	GCPtr<SPhysicsBaseUtility> physics_object;

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
	float mass = 50.f;		//物体质量
	float force_attenuation = 0.85f;		//力衰减
	float bounciness = 0.2f;
	float friction = 0.7f; //摩擦力
	float mass_inv;		//质量相反数
	PhysicsType type;
	SceneComponent* collision_owner;

	//外力
	Vec2<float> added_force;	//冲量

	//结果

	//返回的是碰撞体的左上角，需要自行便宜pivot
	Vec2<float> after_location;
	Rotation after_rotation;


	// Hook
	std::function<void()> physics_callback;

    SPhysicsBaseUtility();
	void Construct() override;
void RegisterDispatchers() override;

	void NativeSetOuter(GCObject *new_outer) override;


    virtual PhysicsType GetPhysicsType(){ return type; }
    [[nodiscard]] FRect GetCollisionBox() const;

	void SetSimulationPhysics(bool is_active);
	void SetPhysicsType(PhysicsType new_type);
    virtual void HandleVelocity(double delta_time);



//位移需要变换位置
	void AddBodyWorldLocation(Vec2<float> v);

    virtual void Init();
    // void SetIsSimulatedPhysics(bool value);
	[[deprecated("Linked when it was created So no need to use")]]
	void SetPhysicsBodyOwner(SceneComponent* new_owner);
	void SetSubscribeCollision(bool is_subscribe);


//物理
	void AddImpulse(const Vec2<float> &force);
	//void AddImpulse(Vec2<float> unit_dir, float force);
//同步
	void SynchronizationTransform();


	// void SetBodyBox(Vec2<float> size);
	void SetBodyTransform(Transform transform);

	void SetBodyWorldLocation(const Vec2<float>& location);
	void SetBodyWorldRotation(const Rotation& rotation);
};