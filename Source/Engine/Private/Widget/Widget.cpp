//
// Created by Administrator on 2025/10/20.
//

#include "Classes/Widget/Widget.hpp"

#include "Types/Enums/WidgetVisibility.hpp"

Widget::Widget()
{

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
    parent->
}

int Widget::GetLayerId() const
{
    return layer_id;
}
