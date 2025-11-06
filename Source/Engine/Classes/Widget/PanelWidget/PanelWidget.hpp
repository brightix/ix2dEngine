#pragma once
#include "Classes/Widget/Slot/PanelSlot.hpp"
#include "Classes/Widget/Widget.hpp"

class PanelWidget : public Widget
{
protected:
    std::vector<GCPtr<PanelSlot>> slots;
public:
    PanelWidget(){}

    virtual std::vector<GCPtr<PanelSlot>> GetSlot()
    {
        std::vector<GCPtr<PanelSlot>> ret(slots.size());
        for (auto& it : slots)
        {
            ret.emplace_back(it);
        }
        return ret;
    }
    void NativeWidgetRender(FRect) final {}
    WidgetType GetWidgetType() override;
};

