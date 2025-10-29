#pragma once
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Widget/Widget.hpp"

class ContentWidget : public Widget
{
protected:
    GCPtr<StaticTexture> widget_texture;
public:
    ContentWidget();
    void PreConstructEvent() override;
    GCWeakPtr<PanelSlot> AddChild(GCPtr<Widget> child) final { return {}; }

    WidgetType GetWidgetType() override;
};
