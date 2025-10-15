#pragma once
#include "SPhysicsUtilityBase.hpp"
#include "Types/Location.hpp"


class MovableActorUtility;

struct WorldPhysics
{
    float GravityForce = -1.f;
};

class SPhysics 
{
	//task
    WorldPhysics world_physics;
    float GravityForce = -1.f;
    //std::vector<SPhysicsTypeBase*> actors;
    std::unordered_set<SPhysicsUtilityBase*> actors;

public:
    SPhysics();


    ~SPhysics()= default;

    void Register(SPhysicsUtilityBase *actor);
    void simulation(float delta_time);
    void HandleVelocity(float delta_time);
    void OnRigidCollision(SPhysicsUtilityBase* A,SPhysicsUtilityBase* B);
};

