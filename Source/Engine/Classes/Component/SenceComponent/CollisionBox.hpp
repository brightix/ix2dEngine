#pragma once
#include "SceneComponent.hpp"
#include "Classes/Core/SPhysics/MovableActorUtility.hpp"

class CollisionBox final : public SceneComponent
{

	GCPtr<SPhysicsBaseUtility> physics_body;
	bool is_outline_visible;
public:
    CollisionBox();
    void Construct() override;
    void ComponentRender() override;
    void SetBoundBox(const Vec2<float>& size);


	//渲染
	void OfferRenderData(std::vector<RenderData>& data) override;
};
