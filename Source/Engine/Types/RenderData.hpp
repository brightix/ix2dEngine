#pragma once
#include <optional>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <memory>
#include "Transform.hpp"
#include "Classes/Component/SenceComponent/SceneComponent.hpp"
#include "Classes/Widget/Widget.hpp"
#include "Classes/Widget/ContentWidget/ContentWidget.hpp"

struct RenderData
{
	int layer;
	std::shared_ptr<SDL_Texture> texture;
	Transform render_transform;
	std::optional<SDL_FRect> src_rect,dst_rect;
	RenderData(std::shared_ptr<SDL_Texture> tex, const Transform& t, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dst = std::nullopt)
		: texture(tex), render_transform(t), src_rect(src), dst_rect(dst)
	{}
	RenderData(SceneComponent* c,std::shared_ptr<SDL_Texture> t)
	{
		layer = static_cast<int>(c->layer);
		texture = t;
		render_transform = c->GetComponentTransform();
		dst_rect = c->GetComponentRenderRect();
	}
};

// struct RenderUMGData
// {
// 	int layer;
// 	std::shared_ptr<SDL_Texture> texture;
// 	std::optional<SDL_FRect> src_rect,dst_rect;
// 	RenderUMGData(std::shared_ptr<SDL_Texture> tex, const Transform& t, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dst = std::nullopt)
// 		: texture(tex), src_rect(src), dst_rect(dst)
// 	{}
// 	RenderUMGData(PanelWidget* w)
// 	{
// 		layer = static_cast<int>(w->layer);
// 		texture = t;
// 		dst_rect = c->GetComponentRenderRect();
// 	}
// };
