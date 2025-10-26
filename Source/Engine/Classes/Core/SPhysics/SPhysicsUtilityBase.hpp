#pragma once
#include <string>

#include "Classes/Object.hpp"
#include "Types/FRect.hpp"
#include "Types/Location.hpp"
#include "Types/Rotation.hpp"
#include "Types/Transform.hpp"
#include "Types/Vec.hpp"

class SceneComponent;

enum class PhysicsType
{
    Static,
    RigidBody,
    Movable
};

class SPhysicsBaseUtility : public GCObject
{
public:
	~SPhysicsBaseUtility() override;
	FRect collision_box;
	Vec2<float> velocity;
	float quality = 10.f;
	float force_attenuation = 1.0f;

	PhysicsType type;
	SceneComponent* owner;
	std::string test_name;


    SPhysicsBaseUtility();
	//void Construct() override;

    virtual PhysicsType GetPhysicsType(){ return type; }
    [[nodiscard]] FRect GetCollisionBox() const;
    virtual void HandleVelocity(double delta_time){}

//位移需要变换位置
	void AddBodyWorldLocation(Vec2<float> v);


    virtual void Init();
    // void SetIsSimulatedPhysics(bool value);
	void SetOwner(SceneComponent* new_owner);


	void SetBodyTransform(Transform transform);

	void SetBodyWorldLocation(const Location& location);
	void SetBodyWorldRotation(const Rotation& rotation);
};