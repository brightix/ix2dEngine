#include "Classes/Component/SenceComponent/SceneTextBlock.hpp"

#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "System/Font.hpp"

SceneTextBlock::SceneTextBlock()
{
    CNAME;
}

void SceneTextBlock::Construct()
{
	SceneComponent::Construct();
}

void SceneTextBlock::SetText(const std::string &new_text)
{
	auto name_tex = MountedComponent(new StaticTexture);
	SetTextureFromSurface_S(name_tex.Peek(), GetTextSurface(new_text));
}
