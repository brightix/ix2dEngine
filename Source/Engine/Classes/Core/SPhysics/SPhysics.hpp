#pragma once
#include <unordered_set>

#include "SPhysicsUtilityBase.hpp"
#include "Utilities/GCWeakPtr.hpp"
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
    float GravityForce = 9.8f;
    //std::vector<SPhysicsTypeBase*> actors;
    std::unordered_set<SPhysicsBaseUtility*> units;
    QuadTree collision_tree;
public:
    SPhysics();


    ~SPhysics()= default;

    void Register(SPhysicsBaseUtility* unit);
	void DeRegister(SPhysicsBaseUtility* unit);
	void simulation(double delta_time);

	void HandlePhysics(double delta_time, SPhysicsBaseUtility* unit);

	//void HandleVelocity(float delta_time);
    static void OnRigidCollision(SPhysicsBaseUtility* A,SPhysicsBaseUtility* B);

	void DebugTree()
	{
		collision_tree.DebugTree();
	}
	//void Clear();
};

