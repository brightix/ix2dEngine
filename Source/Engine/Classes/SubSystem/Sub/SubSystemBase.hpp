#pragma once
#include "Classes/Object.hpp"

class SubSystemBase : public Object
{
public:
	SubSystemBase();
	virtual void Init(){}
	virtual void DeInit(){}

};
