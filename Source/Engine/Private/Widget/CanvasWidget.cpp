#include "../../Classes/Widget/PanelWidget/CanvasWidget.hpp"

#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

void CanvasWidget::AddChild(GCPtr<Widget> child_UI)
{
    Widget::AddChild(child_UI);
    auto slot = NewObject(new CanvasSlot());
    slot->widget = child_UI;
    child_UI.SetOuter(this);



    slots.emplace_back(slot);
    //加入新元素需要刷新
    dirty = true;
    flush();
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

void CanvasWidget::WidgetRender(FRect display_area)
{

    for (auto& slot : slots)
    {
        slot->widget->WidgetRender(slot->display_area);
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
