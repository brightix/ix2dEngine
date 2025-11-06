#include "Classes/Component/SenceComponent/SceneTextBlock.hpp"

#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "System/Font.hpp"

SceneTextBlock::SceneTextBlock()
{
    CNAME;
}

void SceneTextBlock::Construct()
{
	SceneComponent::Construct();
	text_tex = NewObject<Texture>(this);
	text_tex->NativeSetRenderLayer(Foreground);
}

void SceneTextBlock::OfferRenderData(std::vector<RenderData>& data)
{
	//SceneComponent::OfferRenderData(data);
	//data.emplace_back(RenderData(this,text_tex->GetTexture()));
}


void SceneTextBlock::SetText(const std::string &new_text)
{
	SetTextureFromSurface_S(text_tex.Get(), GetTextSurface(new_text));
	w = text_tex->w;
	h = text_tex->h;
}
