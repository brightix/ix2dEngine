#include "Classes/Widget/PanelWidget/PanelWidget.hpp"

#include "Classes/Widget/ContentWidget/ContentWidget.hpp"
#include "Utilities/FuncLib/StaticCast.hpp"


std::vector<GCWeakPtr<PanelSlot>> PanelWidget::GetChildren()
{
    std::vector<GCWeakPtr<PanelSlot>> children_slot;
    for (auto& slot : slots)
    {
        children_slot.emplace_back(slot);
    }
    return children_slot;
}

WidgetType PanelWidget::GetWidgetType()
{
    return WidgetType::PanelWidget;
}
