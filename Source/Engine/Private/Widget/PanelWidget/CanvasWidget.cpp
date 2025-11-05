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


std::vector<GCPtr<PanelSlot>> CanvasWidget::GetSlot()
{
    std::vector<GCPtr<PanelSlot>> s(slots.size());
    for (auto& slot : slots)
    {
        s.emplace_back(slot);
    }
    return s;
}
