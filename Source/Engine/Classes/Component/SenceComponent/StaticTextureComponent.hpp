#pragma once
#include "Asset/Texture.hpp"
#include "Asset/StaticTexture.hpp"

struct RenderData;
class StaticTextureComponent : public SceneComponent
{
protected:
	GCPtr<Texture> s_texture;
public:
	//Vec2<float> transform;
	StaticTextureComponent();
	//纯色
	//explicit StaticTexture(Vec2<int> size, SDL_Color color = Color_White, bool is_fill = false);

	//用图片加载
	StaticTextureComponent(const StaticTextureComponent& other);

	//拷贝构造
	//StaticTexture(StaticTexture& other);

	//StaticTexture(StaticTexture&& other) noexcept ;
	void Construct() override;
	void ComponentEventBegin() override;
	void ComponentRender() override;
	//Get
	[[nodiscard]] Vec2<float> GetSize() const;
	//Set
	//void SetPhysicsType(PhysicsType new_type) const;

	void SetStaticTexture(std::shared_ptr<SDL_Texture> new_texture);
	Texture* GetTexture() const;
	std::shared_ptr<SDL_Texture> GetRowTexture() const;
	//void LoadDefaultTexture(Vec2<int> size, SDL_Color color = {255,255,255,255}, bool is_fill = false);
	//void SetActiveCollision(bool is_active);
	void OfferRenderData(std::vector<RenderData>& data) override;
};

