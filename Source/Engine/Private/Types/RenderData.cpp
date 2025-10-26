#include "Types/RenderData.hpp"
#include "Classes/Component/SenceComponent/SceneComponent.hpp"

RenderData::RenderData(SceneComponent* c, std::shared_ptr<SDL_Texture> t)
{
	layer = static_cast<int>(c->layer);
	texture = t;
	render_transform = c->GetComponentTransform();
	dst_rect = c->GetComponentRenderRect();
}
