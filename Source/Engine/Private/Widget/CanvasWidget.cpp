#include "../../Classes/Widget/PanelWidget/CanvasWidget.hpp"

#include "Classes/Core/RendererCenter.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

void CanvasWidget::AddChild(GCPtr<Widget> child_UI)
{
    Widget::AddChild(child_UI);
    CanvasSlot slot;
    slot.widget = child_UI;
    child_UI.SetOuter(this);



    slots.insert(slot);
    //加入新元素需要刷新
    dirty = true;
    flush();
}

void CanvasWidget::flush()
{
    Widget::flush();
    if (dirty)
    {
        for (auto& it : slots)
        {
            RendererCenter::AddRendererTask();
        }
        dirty = false;
    }

}
