#pragma once
#include "Classes/Widget/UserWidget.hpp"
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"

class BirdWidget : public UserWidget
{
    TextBlockWidget* score_widget;
    CanvasSlot* r;
public:
    BirdWidget() : score_widget(nullptr), r(nullptr) {}

    void RegisterEvents() override;
    void PreConstructEvent() override;
    void ConstructEvent() override;
    void Tick(double delta_time) override;

    void SetScore(int a);

};
