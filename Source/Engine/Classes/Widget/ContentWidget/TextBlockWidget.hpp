#pragma once

#include "ContentWidget.hpp"
#include <string>

#include "Types/FontStyle.hpp"

class TextBlockWidget : public ContentWidget
{
    std::string text;
    FontStyle font_style;
public:
    TextBlockWidget();
    void PreConstructEvent() override;
    void SetText(const std::string& new_text);
    void SetFontStyle(const FontStyle& new_font_style);
    void WidgetRender(FRect display_area) override;
};
