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

class CanvasWidget : public PanelWidget
{
public:
    CanvasWidgetFillRule fill_rule = FillScreen;
    CanvasWidget(){}
    void AddChild(Widget *child) override;

    //继承
    void flush() override;
    std::vector<GCWeakPtr<PanelSlot>> GetSlot() override;
};


