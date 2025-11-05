#include "Classes/Widget/UserWidget.hpp"

void UserWidget::NativeWidgetRender(FRect display_area)
{
    for (auto& it : child_widget | std::views::values)
    {
        it->NativeWidgetRender(display_area);
    }
}

WidgetType UserWidget::GetWidgetType()
{
    return WidgetType::UserWidget;
}

void UserWidget::AddChild(const std::string& widget_name, Widget* child)
{
    auto it = child_widget.find(widget_name);
    if (it != child_widget.end())
    {
        Log("添加Widget重名了");
        return ;
    }
    //添加一个自己的gc关系
    child->outer = this;
    child_widget[widget_name] = child;
}

GCPtr<Widget> UserWidget::GetChild(const std::string &widget_name)
{
    auto it = child_widget.find(widget_name);
    if (it == child_widget.end())
    {
        return {};
    }
    return it->second;
}

std::vector<GCPtr<PanelSlot>> UserWidget::GetChildren()
{
    return Root->widget->GetChildren();
}
