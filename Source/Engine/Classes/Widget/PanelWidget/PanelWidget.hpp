#pragma once
#include "Classes/Widget/Slot/PanelSlot.hpp"
#include "Classes/Widget/Widget.hpp"

class PanelWidget : public Widget
{
protected:
    std::vector<GCPtr<PanelSlot>> slots;
public:
    PanelWidget(){}

    virtual std::vector<GCWeakPtr<PanelSlot>> GetSlot()
    {
        std::vector<GCWeakPtr<PanelSlot>> ret(slots.size());
        for (auto& it : slots)
        {
            ret.emplace_back(it);
        }
        return ret;
    }
    std::vector<GCWeakPtr<PanelSlot>> GetChildren() override;
    WidgetType GetWidgetType() override;
};

