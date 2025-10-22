//
// Created by Administrator on 2025/10/20.
//

#include "Classes/Widget/Widget.hpp"

#include "Types/Enums/WidgetVisibility.hpp"

Widget::Widget() : widget_visibility(WidgetVisibility::Visible), layer_id(0), dirty(true)
{
}

void Widget::Construct()
{
    Object::Construct();
}

WidgetVisibility Widget::GetVisibility()
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
    Widget* p = parent.Peek();
    while (p)
    {
        p->dirty = true;
        p = p->parent.Peek();
    }
}