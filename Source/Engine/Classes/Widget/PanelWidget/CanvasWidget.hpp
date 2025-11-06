#pragma once
#include "PanelWidget.hpp"
#include "../Widget.hpp"
#include "Types/Vec.hpp"


struct CanvasSlot;

enum CanvasWidgetFillRule
{
    FillScreen,
    AutoCalSize,
    CustomSize
};

class CanvasWidget final : public Widget
{
public:
    CanvasWidgetFillRule fill_rule = FillScreen;
    CanvasWidget();
    PanelSlot *CreateSlot() override;
    void ReceiveSlot(PanelSlot* slot) override;
    //继承
    void flush() override;
    void OfferWidgetRenderData(std::vector<RenderData>& data) override;

    void FlushDirty() override;
};


