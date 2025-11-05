#include "public/TestPawn.hpp"

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Enum/ActorEnum.hpp"

TestPawn::TestPawn()
{
    CNAME;
}

void TestPawn::Construct()
{
	Pawn::Construct();

	default_texture = GetSceneComponent("default_texture").Cast<StaticTexture>();
	default_texture->SetNewTexture(Create_FilledTexture_S({100,100}));
	default_texture->SetActiveCollision(true);
	default_texture->SetPhysicsType(PhysicsType::Movable);

	// default_texture->
	// collision_box = Root->MountedComponent(new CollisionBox());
	// collision_box->SetBoundBox({st->w,st->h});
}

void TestPawn::EventBegin()
{
	auto t = dynamic_cast<Object*>(default_texture.Peek());
	Pawn::EventBegin();
	// ListenDispatcher(this,"OnNameChanged",Event([this](TEventParams e) {
	// 	default_texture->SetOwner(this);
	// }));
	// ListenDispatcher(default_texture.Peek(),"OnComponentLocationChanged",Event([this](TEventParams e) {
	//
	// }));
}

