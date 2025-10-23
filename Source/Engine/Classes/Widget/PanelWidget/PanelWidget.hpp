#pragma once
#include <unordered_set>

#include "Classes/Widget/Slot/PanelSlot.hpp"


class PanelWidget : public Widget
{
protected:

public:
    PanelWidget();

    virtual std::vector<GCWeakPtr<PanelSlot>> GetSlot() { return {}; }
};

