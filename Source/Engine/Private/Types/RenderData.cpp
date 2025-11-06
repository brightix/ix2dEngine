#include "Types/RenderData.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Component/SenceComponent/SceneComponent.hpp"
#include "Classes/Widget/Widget.hpp"
RenderData::RenderData(SceneComponent* c, std::shared_ptr<SDL_Texture> t)
{
	layer = static_cast<int>(c->layer);
	texture = t;
	render_transform = c->GetComponentTransform();
	dst_rect = c->GetComponentRenderRect();
}

RenderData::RenderData(Widget *w, std::shared_ptr<SDL_Texture> t)
{
	layer = w->layer_id;
	texture = t;
	auto slot = w->GetParentSlot();
	const auto& area = slot ->display_area;
	if (slot->fill_method == Auto)
	{
		dst_rect = SDL_FRect(area.x, area.y, t->w, t->h);
	}
	else
	{
		dst_rect = {area.x, area.y,min(t->w, area.w),min(t->h, area.h)};
	}
}
