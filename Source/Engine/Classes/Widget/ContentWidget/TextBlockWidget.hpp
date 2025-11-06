#pragma once

#include "ContentWidget.hpp"
#include <string>

#include "Types/FontStyle.hpp"

class TextBlockWidget final : public Widget
{
    std::string text;
    FontStyle font_style;
    GCPtr<Texture> text_tex;
public:
    TextBlockWidget();
    void PreConstructEvent() override;
    void SetText(const std::string& new_text);
    void SetFontStyle(const FontStyle& new_font_style);
    void WidgetRender(FRect display_area) override;
    void OfferWidgetRenderData(std::vector<RenderData> &data) override;

    PanelSlot * CreateSlot() override { return nullptr;}

    void ReceiveSlot(PanelSlot *slot) override {}

    void FlushDirty() override {}
};
