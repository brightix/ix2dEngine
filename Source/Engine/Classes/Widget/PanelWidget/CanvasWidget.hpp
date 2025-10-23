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
    std::vector<GCPtr<CanvasSlot>> slots;
public:
    CanvasWidgetFillRule fill_rule = FillScreen;
    CanvasWidget();
    void AddChild(GCPtr<Widget> child_UI) override;


    //继承
    void flush() override;
    void WidgetRender(FRect display_area) override;
    std::vector<GCWeakPtr<PanelSlot>> GetSlot() override;
};


