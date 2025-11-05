#pragma once
#include "Classes/Actor.hpp"

class TestGround : public Actor
{
public:
    TestGround();

    TestGround(Transform trans);

    void Construct() override;

    void SetGroundSize(const Vec2<float> &new_size) const;
};
