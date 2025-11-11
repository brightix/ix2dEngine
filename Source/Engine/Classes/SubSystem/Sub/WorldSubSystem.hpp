#pragma once
#include "SubsystemBase.hpp"


class WorldSubSystem : public SubsystemBase
{
public:
    WorldSubSystem(){}
    ~WorldSubSystem() override = default;
	void Init() override;
};

