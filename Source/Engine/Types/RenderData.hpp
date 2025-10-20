#pragma once
#include <optional>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "Transform.hpp"

struct RenderData
{
	std::shared_ptr<SDL_Texture> texture;
	Transform render_transform;
	std::optional<SDL_FRect> src_rect,dst_rect;
	RenderData(std::shared_ptr<SDL_Texture> tex, const Transform& t, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dst = std::nullopt)
		: texture(tex), render_transform(t), src_rect(src), dst_rect(dst) {}
};
