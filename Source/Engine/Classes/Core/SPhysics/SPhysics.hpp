#pragma once
#include <unordered_set>

#include "SPhysicsUtilityBase.hpp"
#include "Utilities/GCPtr.hpp"
#include "Utilities/QuadTree.hpp"


class MovableUtility;

struct WorldPhysics
{
    Vec2<float> GravityDir = {0,1};
};

class SPhysics : public EngineSubSystem
{
	//task
    WorldPhysics world_physics;
    float WorldGravityForce = 9.8f;
    //std::vector<SPhysicsTypeBase*> actors;
    QuadTree collision_tree;
public:
    std::unordered_set<SPhysicsBaseUtility*> units;
    SPhysics();


    ~SPhysics() override;

    void Register(SPhysicsBaseUtility* unit);
	void DeRegister(SPhysicsBaseUtility* unit);

	void SimulationTunneling(double delta_time);

	void Simulation(double delta_time);

	void HandlePhysics(double delta_time, SPhysicsBaseUtility* unit) const;
	void Synchronization() const;


	//void HandleVelocity(float delta_time);
    static void OnRigidCollision(SPhysicsBaseUtility* A,SPhysicsBaseUtility* B);

	void OnStaticBodyCollision(SPhysicsBaseUtility* A, const SPhysicsBaseUtility* B);

	void OnMovableBodyCollision(SPhysicsBaseUtility* A, SPhysicsBaseUtility* B);

	void DebugTree()
	{
#if DEBUG == 1
		TStart;
		collision_tree.DebugTree();
		TEnd;
#endif
	}
	//void Clear();
};

