#pragma once
#include "SceneComponent.hpp"

class Capsule2d final : public SceneComponent
{
    GCPtr<SPhysicsBaseUtility> physics_body;
	bool simulation_physics;
public:
    Capsule2d();

    void SetActiveCollision(bool is_active);
};
