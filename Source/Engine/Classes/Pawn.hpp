#pragma once
#include "Classes/Actor.hpp"
#include "SubSystem/EnhancedInputSubSystem.hpp"

class MovableComponent;

class Pawn : public Actor
{
	float base_move_speed;
public:
	GCPtr<EnhancedInputSubSystem> enhanced_input_sub_system;
	using Actor::Actor;
    ~Pawn() override;
	void Construct() override;
	void EventBegin() override;
    void Tick(double deltaTime) override;

	//增强输入
	void CallEnhancedInputEventBool(EnhancedInputParam<bool> param) const;
	void CallEnhancedInputEventDouble(EnhancedInputParam<double> param);
	[[nodiscard]] EnhancedInputSubSystem* GetEnhancedInputSubSystem() const;
	[[nodiscard]] Vec2<float> GetVelocity_Debug() const;
};

