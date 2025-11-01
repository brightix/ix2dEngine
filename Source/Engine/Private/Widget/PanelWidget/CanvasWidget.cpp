#include "../../../Classes/Widget/PanelWidget/CanvasWidget.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

CanvasWidget::CanvasWidget()
{
	CNAME;
}

GCWeakPtr<PanelSlot> CanvasWidget::AddChild(GCPtr<Widget> child)
{
    //Widget::AddChild(child);

    auto slot = NewObject(new CanvasSlot());
	slot->widget = child;
    child.SetOuter(slot.Get());
    if (is_initialized && !child->is_initialized)
    {
        child->WidgetEventBegin();
    }
    else
    {
        World()->BindEventToDispatcher(this, "EventBegin", Event([child](TEventParams e) {
            child->WidgetEventBegin();
        }));
    }

    slots.emplace_back(slot);
    //加入新元素需要刷新
    dirty = true;
    return slot;
}

void CanvasWidget::flush()
{
    Widget::flush();
    for (auto& slot : slots)
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


std::vector<GCWeakPtr<PanelSlot>> CanvasWidget::GetSlot()
{
    std::vector<GCWeakPtr<PanelSlot>> s(slots.size());
    for (auto& slot : slots)
    {
        s.emplace_back(slot);
    }
    return s;
}
