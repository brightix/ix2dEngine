#include "../../../Classes/Widget/PanelWidget/CanvasWidget.hpp"

#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

void CanvasWidget::AddChild(Widget* child)

{
    Widget::AddChild(child);


    auto slot = NewObject(new CanvasSlot());
    slot->widget = CreateWidget(child);

    if (World()->is_simulation)
    {
        child->WidgetEventBegin();
    }
    else
    {
        World()->BindEvent(this, "EventBegin", Event([child](TEventParams e) {
            child->WidgetEventBegin();
        }));
    }

    slots.emplace_back(slot);
    //加入新元素需要刷新
    dirty = true;
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
