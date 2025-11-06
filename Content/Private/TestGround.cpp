#include "Public/TestGround.hpp"

#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"

void TestGround::Construct()
{
    Actor::Construct();
    CNAME;
    st = Cast<StaticTextureComponent>(GetSceneComponent("default_texture"));
    st->SetComponentName("Ground");
    st->NativeSetActiveCollision(true);
    //s_t->SetPhysicsType(PhysicsType::Static);
    SetActorName("Ground");
    SetGroundSize({1000,1000});
}

void TestGround::SetGroundSize(const Vec2<float> &new_size) const
{
    Cast<StaticTextureComponent>(GetSceneComponent("Ground"))->SetStaticTexture(Create_FilledTexture_S(new_size));
}
