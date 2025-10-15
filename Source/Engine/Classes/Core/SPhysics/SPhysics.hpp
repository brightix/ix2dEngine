#pragma once
#include <unordered_set>

#include "SPhysicsUtilityBase.hpp"
#include "Utilities/QuadTree.hpp"


class MovableUtility;

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
    std::unordered_set<SPhysicsBaseUtility*> actors;
    QuadTree collision_tree;
public:
    SPhysics();


    ~SPhysics()= default;

    void Register(SPhysicsBaseUtility *actor);
    void simulation(float delta_time);
    void HandleVelocity(float delta_time);
    static void OnRigidCollision(SPhysicsBaseUtility* A,SPhysicsBaseUtility* B);

};

