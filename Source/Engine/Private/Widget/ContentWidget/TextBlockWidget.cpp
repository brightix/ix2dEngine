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
    ContentWidget::PreConstructEvent();
}


void TextBlockWidget::SetText(const std::string& new_text)
{
    text = new_text;
    auto text_surface = GetTextSurface(text,font_style);
	auto t = widget_texture.Get();
    SetTextureFromSurface_S(t, text_surface);
}

void TextBlockWidget::SetFontStyle(const FontStyle &new_font_style)
{
    font_style = new_font_style;
    auto text_surface = GetTextSurface(text,font_style);

    SetTextureFromSurface_S(widget_texture.Get(), text_surface);
}

void TextBlockWidget::WidgetRender(FRect display_area)
{
    //ContentWidget::WidgetRender();
    auto dst = SDL_FRect(display_area.x,display_area.y,widget_texture->w,widget_texture->h);
    SDL_RenderTexture(GetRenderer(),widget_texture->GetTexture().get(),nullptr,&dst);
}



