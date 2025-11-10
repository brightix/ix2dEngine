#include "Public/TestGround.hpp"

#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"

void TestGround::Construct()
{
    Actor::Construct();
    CNAME;
    st = Cast<StaticTextureComponent>(GetSceneComponent("default_texture"));
    st->NativeSetActiveCollision(true);
    //s_t->SetPhysicsType(PhysicsType::Static);
    //SetActorName("Unknown_");
	size = {1000,1000};
    SetGroundSizeAndColor(size);
}

StaticTextureComponent* TestGround::GetGroundTexture()
{
	return st.Get();
}

void TestGround::SetGroundSizeAndColor(const Vec2<float> &new_size, SDL_Color color)
{
	size = new_size;
    Cast<StaticTextureComponent>(GetSceneComponent("default_texture"))->SetStaticTexture(Create_FilledTexture_S(new_size,color));
}
