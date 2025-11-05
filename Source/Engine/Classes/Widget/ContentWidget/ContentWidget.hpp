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
    PanelSlot* AddChild(Widget *) final { return {}; }
    PanelSlot *CreateSlot() final { return nullptr; }
    WidgetType GetWidgetType() override;
    void ReceiveSlot(PanelSlot* slot) final {}
};
