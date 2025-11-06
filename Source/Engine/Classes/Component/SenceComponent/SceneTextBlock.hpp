#pragma once
#include "SceneComponent.hpp"
#include "StaticTextureComponent.hpp"


class SceneTextBlock : public SceneComponent
{
	std::string text;
	GCPtr<Texture> text_tex;
public:
    SceneTextBlock();
	void Construct() override;
	void OfferRenderData(std::vector<RenderData>& data) override;

	void SetText(const std::string& new_text);
};

