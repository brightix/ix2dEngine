//
// Created by Administrator on 2025/10/20.
//

#include "Classes/Widget/Widget.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Types/Enums/WidgetVisibility.hpp"

Widget::Widget() : widget_visibility(WidgetVisibility::Visible), layer_id(0), dirty(true)
{
}


void Widget::ForTick(double delta_time)
{
    Tick(delta_time);
    auto children = GetChildren();
    for (auto& child : children)
    {
        child->widget->Tick(delta_time);
    }
}

void Widget::NativeWidgetRender(FRect display_area)
{
    WidgetRender(display_area);
    auto children = GetChildren();
    for (auto& child : children)
    {
        child->widget->NativeWidgetRender(child->display_area);
    }
}

PanelSlot* Widget::AddChild(Widget* child)
{
    PanelSlot* slot = CreateSlot();

    slot->widget = child;
	child->parent_slot = slot;

    child->outer = slot;
    if (is_initialized && !child->is_initialized)
    {
        child->WidgetEventBegin();
    }
    else
    {
        child->ListenDispatcher(World(), "EventBegin", Event([child](TEventParams e) {
            child->WidgetEventBegin();
        }));
    }
    //加入新元素需要刷新
    dirty = true;
    ReceiveSlot(slot);
    return slot;
}

// GCPtr<PanelSlot> Widget::AddChild(GCPtr<Widget> child);

WidgetVisibility Widget::GetVisibility() const
{
    return widget_visibility;
}

void Widget::SetVisibility(const WidgetVisibility new_Visibility)
{
    widget_visibility = new_Visibility;
}

void Widget::RemoveFromParent()
{
    dirty = true;

}


int Widget::GetLayerId() const
{
    return layer_id;
}

void Widget::MakeDirty()
{
    dirty = true;
    Widget* p = parent_widget.Peek();
    while (p)
    {
        p->dirty = true;
        p = p->parent_widget.Peek();
    }
}