#pragma once
#include "Widget.hpp"


class UserWidget : public Widget
{
protected:

	std::unordered_map<std::string, GCPtr<Widget>> child_widget;
	//GCPtr<Widget> Root;
public:
    UserWidget(){}
    virtual void Tick(double delta_time) override{}
    void NativeWidgetRender(FRect display_area) override;
    WidgetType GetWidgetType() override;
    void AddChild(const std::string &widget_name, GCPtr<Widget> child);
    GCWeakPtr<Widget> GetChild(const std::string &widget_name);
    std::vector<GCWeakPtr<PanelSlot>> GetChildren() override;


    GCWeakPtr<PanelSlot> AddChild(GCPtr<Widget> child) final { return {};}
};

