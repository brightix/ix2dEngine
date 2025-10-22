#pragma once
#include "Classes/Component/SenceComponent/StaticTexture.hpp"
#include "Classes/Widget/Widget.hpp"

class ContentWidget : public Widget
{
protected:
    GCPtr<StaticTexture> widget_texture;
public:
    ContentWidget();
    void Construct() override;
	void WidgetRender() override {}
};
