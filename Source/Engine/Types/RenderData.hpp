#pragma once
#include <optional>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <memory>
#include "Transform.hpp"
#include "Classes/Component/SenceComponent/SceneComponent.hpp"

class Widget;

struct RenderData
{
	int layer;
	std::shared_ptr<SDL_Texture> texture;
	Transform render_transform;
	std::optional<SDL_FRect> src_rect,dst_rect;
	RenderData(std::shared_ptr<SDL_Texture> tex, const Transform& t, const std::optional<SDL_FRect>& src = std::nullopt,
	           const std::optional<SDL_FRect>& dst = std::nullopt, const int layer_ = 0)
		: layer(layer_), texture(tex), render_transform(t), src_rect(src), dst_rect(dst)
	{ }

	RenderData(SceneComponent* c,std::shared_ptr<SDL_Texture> t);
	RenderData(Widget* w,std::shared_ptr<SDL_Texture> t);
};
