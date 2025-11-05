#include "Classes/Component/SenceComponent/Capsule2d.hpp"

Capsule2d::Capsule2d()
{
    CNAME;
}

void Capsule2d::SetActiveCollision(const bool is_active)
{
    simulation_physics = is_active;
}


