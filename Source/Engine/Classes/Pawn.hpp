#pragma once
#include "Classes/Actor.hpp"
#include "SubSystem/EnhancedInputSubSystem.hpp"

class MovableComponent;

class Pawn : public Actor
{
	//Vec2d<float> player_input_Vec;
	float base_move_speed;
	//GCPtr<> sprite;
protected:
public:
	GCPtr<EnhancedInputSubSystem> enhanced_input_sub_system;
	using Actor::Actor;
    ~Pawn() override;
	void Construct() override;
	void EventBegin() override;
    void Tick(double deltaTime) override;

	//增强输入
	void CallEnhancedInputEventBool(EnhancedInputParam<bool> param);
	void CallEnhancedInputEventDouble(EnhancedInputParam<double> param);
	EnhancedInputSubSystem* GetEnhancedInputSubSystem();
	Vec2<float> GetVelocity_Debug() const;
};

