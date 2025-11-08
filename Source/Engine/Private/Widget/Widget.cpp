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

void Widget::Construct()
{
    Object::Construct();
}


void Widget::ForTick(double delta_time)
{
    Tick(delta_time);
    auto children = GetSlots();
    for (auto& child : children)
    {
        child->widget->Tick(delta_time);
    }
}

void Widget::NativeWidgetRender(FRect display_area)
{
    WidgetRender(display_area);
    auto children = GetSlots();
    for (auto& child : children)
    {
        child->widget->NativeWidgetRender(child->display_area);
    }
}

void Widget::NativeOfferRenderData(std::vector<RenderData>& data)
{
    OfferWidgetRenderData(data);

    auto children = GetSlots();
    for (auto& child : children)
    {
        child->widget->NativeOfferRenderData(data);
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
        child->ListenDispatcher(World(), "OnEventBegin", "EventBegin");
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

PanelSlot * Widget::GetParentSlot() const
{
    return parent_slot.Get();
}

std::vector<PanelSlot *> Widget::GetSlots() const
{
    std::vector<PanelSlot*> s;
    s.reserve(slots.size());
    for (auto& slot : slots)
    {
        s.emplace_back(slot.Get());
    }
    return s;
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

void Widget::FlushDirty()
{
    int n = slots.size();
    for (int i = 0; i < n; ++i)
    {

    }

    for (auto& slot : slots)
    {

    }
}
