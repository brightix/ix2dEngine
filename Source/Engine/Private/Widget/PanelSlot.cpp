#include "Classes/Widget/Slot/PanelSlot.hpp"

#include "Classes/Widget/Widget.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

PanelSlot::PanelSlot()
{

}

GCWeakPtr<Widget> PanelSlot::GetWidget()
{
    return widget;
}

