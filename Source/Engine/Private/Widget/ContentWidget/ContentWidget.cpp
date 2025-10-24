#include "Classes/Widget/ContentWidget/ContentWidget.hpp"

ContentWidget::ContentWidget()
{

}

void ContentWidget::PreConstructEvent()
{
    Widget::Construct();
    widget_texture = NewObject(new StaticTexture);
}

WidgetType ContentWidget::GetWidgetType()
{
    return WidgetType::ContentWidget;
}
