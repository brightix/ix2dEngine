#include "public/TestPawn.hpp"

#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Enum/ActorEnum.hpp"

void TestPawn::Construct()
{
	Pawn::Construct();
	CNAME;
	default_texture = Cast<StaticTextureComponent>(GetSceneComponent("default_texture"));
	default_texture->SetStaticTexture(Create_FilledTexture_S({100,100}));
	default_texture->NativeSetActiveCollision(true);
	default_texture->NativeSetPhysicsType(PhysicsType::Movable);

	// default_texture->
	// collision_box = Root->MountedComponent(new CollisionBox());
	// collision_box->SetBoundBox({st->w,st->h});
}

void TestPawn::EventBegin()
{
	//auto t = dynamic_cast<Object*>(default_texture.Peek());
	Pawn::EventBegin();
	// ListenDispatcher(this,"OnNameChanged",Event([this](TEventParams e) {
	// 	default_texture->SetOwner(this);
	// }));
	// ListenDispatcher(default_texture.Peek(),"OnComponentLocationChanged",Event([this](TEventParams e) {
	//
	// }));
}

