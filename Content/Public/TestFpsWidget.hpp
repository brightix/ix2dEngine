#pragma once
#include "Classes/Widget/UserWidget.hpp"

class TestFps final : public UserWidget
{

public:
    TestFps();
    void PreConstructEvent() override;
    void Tick(double delta_time) override;
};
