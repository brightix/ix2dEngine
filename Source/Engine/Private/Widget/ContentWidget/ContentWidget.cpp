#include "Classes/Widget/ContentWidget/ContentWidget.hpp"

ContentWidget::ContentWidget()
{

}

void ContentWidget::PreConstructEvent()
{
    widget_texture = NewObject<StaticTexture>(this);
}

WidgetType ContentWidget::GetWidgetType()
{
    return WidgetType::ContentWidget;
}
