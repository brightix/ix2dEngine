#pragma once
#include "Classes/Core/GCObject.hpp"


class SubSystemBase : public GCObject
{
public:
	SubSystemBase();
	virtual void Init(){}
	virtual void DeInit(){}
	virtual ~SubSystemBase() = default;
};
