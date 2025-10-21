#pragma once
#include <unordered_set>

#include "Classes/Widget/Slot/PanelSlot.hpp"


class PanelWidget : public Widget
{
    std::unordered_set<PanelSlot> slots;
public:
    PanelWidget();
    ~PanelWidget()= default;
};

