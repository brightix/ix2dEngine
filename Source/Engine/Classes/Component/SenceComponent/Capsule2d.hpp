#pragma once
#include <unordered_set>

#include "SceneComponent.hpp"

class Capsule2d final : public SceneComponent
{
public:
    Capsule2d();
    void Construct() override;
	void RegisterEvents() override;
	void RegisterDispatchers() override;
    void ComponentEventBegin() override;
    void NativeSetActiveCollision(bool is_active) override;
	void Collision(std::unordered_set<SPhysicsBaseUtility*> cols);
};
