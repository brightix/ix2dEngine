#include "Public/TestGround.hpp"

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"


TestGround::TestGround()
{
    CNAME;
}
TestGround::TestGround(Transform trans) : Actor(trans)
{
    CNAME;
}

void TestGround::Construct()
{
    Actor::Construct();
    auto s_t = GetSceneComponent("default_texture").Cast<StaticTexture>();
    s_t->SetComponentName("Ground");
    s_t->SetActiveCollision(true);
    //s_t->SetPhysicsType(PhysicsType::Static);
    SetActorName("Ground");
    SetGroundSize({1000,1000});
}

void TestGround::SetGroundSize(const Vec2<float> &new_size) const
{
    GetSceneComponent("Ground").Cast<StaticTexture>()->SetNewTexture(Create_FilledTexture_S(new_size));
}
