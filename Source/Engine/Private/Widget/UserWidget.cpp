#include "Classes/Widget/UserWidget.hpp"

#include "Classes/Widget/Slot/UserWidgetSlot.hpp"

void UserWidget::NativeWidgetRender(FRect display_area)
{
    // for (auto& it : child_widget | std::views::values)
    // {
    //     it->NativeWidgetRender(display_area);
    // }
}

void UserWidget::PreConstructEvent()
{
    Widget::PreConstructEvent();
}

void UserWidget::OfferWidgetRenderData(std::vector<RenderData> &data)
{
    Widget::OfferWidgetRenderData(data);

}


WidgetType UserWidget::GetWidgetType()
{
    return WidgetType::UserWidget;
}

// void UserWidget::AddChild(const std::string& widget_name, Widget* child)
// {
//     auto it = child_widget.find(widget_name);
//     if (it != child_widget.end())
//     {
//         Log("添加Widget重名了");
//         return ;
//     }
//     //添加一个自己的gc关系
//     child->outer = this;
//     child_widget[widget_name] = child;
// }

// GCPtr<Widget> UserWidget::GetChild(const std::string &widget_name)
// {
//     auto it = child_widget.find(widget_name);
//     if (it == child_widget.end())
//     {
//         return {};
//     }
//     return it->second;
// }

PanelSlot * UserWidget::CreateSlot()
{
    if (Root)
    {
        //一个UserWidget只能有一个根
        LogWithLevel(FatalError,"重复添加用户控件");
        return nullptr;
    }
    Root = NewObject<UserWidgetSlot>();
    return Root.Get();

}

void UserWidget::ReceiveSlot(PanelSlot *slot)
{
    slots.emplace_back(slot);
}

void UserWidget::FlushDirty()
{
}
