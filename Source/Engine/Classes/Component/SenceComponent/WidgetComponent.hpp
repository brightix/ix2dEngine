#pragma once
#include "SceneComponent.hpp"

class UserWidget;

class WidgetComponent : public SceneComponent
{
    GCPtr<UserWidget> widget;
public:
    WidgetComponent();
    void Construct() override;

    void SetWidget(UserWidget* new_widget);
    void ComponentTick(double delta_time) override;
    void OfferRenderData(std::vector<RenderData>& data) override ;
};
