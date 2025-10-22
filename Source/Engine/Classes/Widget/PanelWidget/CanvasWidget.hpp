#pragma once
#include "PanelWidget.hpp"
#include "../Widget.hpp"
#include "Types/Vec.hpp"


struct CanvasSlot;

class CanvasWidget : public PanelWidget
{

    std::vector<PanelSlot> slots;
public:

    CanvasWidget();
    void AddChild(GCPtr<Widget> child_UI) override;


    //继承
    void flush() override;
};


