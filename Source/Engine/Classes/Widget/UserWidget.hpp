#pragma once
#include "Widget.hpp"


class UserWidget : public Widget
{
protected:

	std::unordered_map<std::string, GCPtr<Widget>> child_widget;
	GCPtr<PanelSlot> Root;
public:
    UserWidget(){}
    void Tick(double delta_time) override {}
    void NativeWidgetRender(FRect display_area) override;
    WidgetType GetWidgetType() override;
    void AddChild(const std::string &widget_name, Widget *child);
    GCPtr<Widget> GetChild(const std::string &widget_name);
    std::vector<GCPtr<PanelSlot>> GetChildren() override;
	PanelSlot *CreateSlot() final { return Root.Get();}
	void ReceiveSlot(PanelSlot* slot) final {}
};

