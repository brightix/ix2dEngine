#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "System/Font.hpp"
#include "Types/FontStyle.hpp"
TextBlockWidget::TextBlockWidget()
{
	CNAME;
    text = "未命名";
}

void TextBlockWidget::PreConstructEvent()
{
    Widget::PreConstructEvent();
    text_tex = NewObject<Texture>(this);
    layer_id = -1;
}


void TextBlockWidget::SetText(const std::string& new_text)
{
    text = new_text;
    auto text_surface = GetTextSurface(text,font_style);
	auto t = text_tex.Get();
    SetTextureFromSurface_S(t, text_surface);
}

void TextBlockWidget::SetFontStyle(const FontStyle &new_font_style)
{
    font_style = new_font_style;
    auto text_surface = GetTextSurface(text,font_style);

    SetTextureFromSurface_S(text_tex.Get(), text_surface);
}

void TextBlockWidget::WidgetRender(FRect display_area)
{
    //ContentWidget::WidgetRender();
    //SDL_RenderTexture(GetRenderer(),widget_texture->GetRowTexture().get(),nullptr,&dst);
}

void TextBlockWidget::OfferWidgetRenderData(std::vector<RenderData> &data)
{
    data.emplace_back(this,text_tex->GetTexture());
}



