#pragma once
#include "Texture.hpp"

class CollisionBox;
struct RenderData;
class StaticTexture : public Texture
{
	//GCWeakPtr<CollisionBox> collision_box;
	GCPtr<SPhysicsBaseUtility> physics_body;
	bool simulation_physics;

public:
	//Vec2<float> transform;
	StaticTexture();
	//纯色
	//explicit StaticTexture(Vec2<int> size, SDL_Color color = Color_White, bool is_fill = false);

	//用图片加载
	StaticTexture(const StaticTexture& other);

	//拷贝构造
	//StaticTexture(StaticTexture& other);

	//StaticTexture(StaticTexture&& other) noexcept ;
	void Construct() override;
	void ComponentEventBegin() override;
	void ComponentRender() override;
	//Get
	TextureType GetTextureType() override;
	GCWeakPtr<SPhysicsBaseUtility> GetPhysicsBody();
	//Set
	void SetActiveCollision(bool is_active, PhysicsType new_type = PhysicsType::Static) const;
	void SetPhysicsType(PhysicsType new_type) const;


	//void LoadDefaultTexture(Vec2<int> size, SDL_Color color = {255,255,255,255}, bool is_fill = false);
	//void SetActiveCollision(bool is_active);
	void OfferRenderData(std::vector<RenderData>& data) override;
};

