#pragma once
#include "Widget.hpp"


/**
 * 所有的重写类都必须继承自用户控件
 */
class UserWidget : public Widget
{
protected:

	//std::unordered_map<std::string, GCPtr<Widget>> child_widget;
	GCPtr<PanelSlot> Root;
public:
    UserWidget(){}
    void Tick(double delta_time) override {}
    void NativeWidgetRender(FRect display_area) override;
	void PreConstructEvent() override;
	void OfferWidgetRenderData(std::vector<RenderData> &data) override;

    WidgetType GetWidgetType() override;

    //void AddChild(const std::string &widget_name, Widget *child);
    GCPtr<Widget> GetChild(const std::string &widget_name);

	PanelSlot *CreateSlot() final;
	void ReceiveSlot(PanelSlot* slot) final ;

	void FlushDirty() override;
};

