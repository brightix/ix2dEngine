#include "Classes/Component/SenceComponent/WidgetComponent.hpp"
#include "Classes/Widget/UserWidget.hpp"
WidgetComponent::WidgetComponent()
{
    CNAME;
}

void WidgetComponent::Construct()
{
    SceneComponent::Construct();
}

void WidgetComponent::SetWidget(UserWidget *new_widget)
{
    widget = new_widget;
    widget->layer_id = Foreground;
}


void WidgetComponent::ComponentTick(const double delta_time)
{
    SceneComponent::ComponentTick(delta_time);
    widget->Tick(delta_time);
}

void WidgetComponent::OfferRenderData(std::vector<RenderData> &data)
{
    //SceneComponent::OfferRenderData(data);
    widget->NativeOfferRenderData(data);
}




