#pragma once
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"

class TestFpsWidget final : public TextBlockWidget
{

public:
    TestFpsWidget();
    void Construct() override;
    void Tick(double delta_time) override;
};
