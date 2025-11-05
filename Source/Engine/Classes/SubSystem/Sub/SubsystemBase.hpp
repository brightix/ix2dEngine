#pragma once
#include "Classes/Object.hpp"

class SubsystemBase : public Object
{
public:
	SubsystemBase();
	virtual void Init(){}
	virtual void DeInit(){}

};
