#pragma once
#include "Classes/Actor.h"
#include "SubSystem/EnhancedInputSubSystem.hpp"
#include "System/Texture.hpp"

class Pawn : public Actor
{
	GCPtr<EnhancedInputSubSystem> enhanced_input_sub_system;
	//GCPtr<> sprite;
public:
    Pawn();
    ~Pawn() override;
    void Tick(double deltaTime) override;

	//增强输入
	void CallEnhancedInputEventBool(EnhancedInputParam<bool> param);
	void CallEnhancedInputEventDouble(EnhancedInputParam<double> param);
};

