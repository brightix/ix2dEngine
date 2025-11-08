#pragma once
#include "SceneComponent.hpp"

class Capsule2d final : public SceneComponent
{
public:
    Capsule2d();
    void Construct() override;
	void RegisterEvents() override;
    void ComponentEventBegin() override;
    void NativeSetActiveCollision(bool is_active) override;
};
