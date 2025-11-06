#include "../../../Classes/Widget/PanelWidget/CanvasWidget.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

CanvasWidget::CanvasWidget()
{
	CNAME;
}

PanelSlot * CanvasWidget::CreateSlot()
{
    return NewObject(new CanvasSlot,this);
}

void CanvasWidget::ReceiveSlot(PanelSlot* slot)
{
    slots.emplace_back(slot);
}


void CanvasWidget::flush()
{
    Widget::flush();
    for (const auto& slot : slots)
    {
        if (slot->widget->dirty)
        {
            //slot.
        }
    }
    if (dirty)
    {
        // for (auto& it : slots)
        // {
        //     RendererCenter::AddRendererTask();
        // }
        dirty = false;
    }
}

void CanvasWidget::OfferWidgetRenderData(std::vector<RenderData>& data)
{
    // for (auto& slot : slots)
    // {
    //     slot->widget->OfferWidgetRenderData(data);
    // }
}

void CanvasWidget::FlushDirty()
{

}
