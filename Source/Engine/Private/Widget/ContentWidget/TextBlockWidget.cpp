#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "System/Font.hpp"

void TextBlockWidget::Construct()
{
    ContentWidget::Construct();
}


void TextBlockWidget::SetText(const std::string& new_text)
{
    text = new_text;
    auto text_surface = FontRenderer::GetTextSurface(text,font_style);
    RendererCenter::SetTextureFromSurface(widget_texture.Get(), text_surface);
}

void TextBlockWidget::SetFontStyle(const FontStyle &new_font_style)
{
    font_style = new_font_style;
    auto text_surface = FontRenderer::GetTextSurface(text,font_style);
    RendererCenter::SetTextureFromSurface(widget_texture.Get(), text_surface);
}

void TextBlockWidget::WidgetRender(FRect display_area)
{
    //ContentWidget::WidgetRender();
    auto dst = SDL_FRect(500,500,widget_texture->w,widget_texture->h);
    SDL_RenderTexture(GetRenderer(),widget_texture->GetTexture().get(),nullptr,&dst);
}



