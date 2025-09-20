#pragma once
#include "Classes/Actor.h"
#include "SubSystem/EnhancedInputSubSystem.hpp"
#include "System/Texture.hpp"

class Pawn : public Actor
{
	GCPtr<EnhancedInputSubSystem> enhanced_input_sub_system;
	Vec2d<float> player_input_Vec;
	float base_move_speed;
	//GCPtr<> sprite;
public:
    Pawn();
    ~Pawn() override;
	void Construct() override;
    void Tick(double deltaTime) override;

	//增强输入
	void CallEnhancedInputEventBool(EnhancedInputParam<bool> param);
	void CallEnhancedInputEventDouble(EnhancedInputParam<double> param);
};

