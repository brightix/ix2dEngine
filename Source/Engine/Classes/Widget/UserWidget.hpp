#pragma once
#include "Widget.hpp"


class UserWidget : public Widget
{
    std::unordered_map<std::string, GCPtr<Widget>> child_widget;
protected:

public:
    UserWidget(){}
    virtual void Tick(double delta_time) override{}
    void NativeWidgetRender(FRect display_area) override;
    WidgetType GetWidgetType() override;
    void AddChild(const std::string &widget_name, GCPtr<Widget> child);
    GCWeakPtr<Widget> GetChild(const std::string &widget_name);
    std::vector<GCWeakPtr<PanelSlot>> GetChildren() override;
};

