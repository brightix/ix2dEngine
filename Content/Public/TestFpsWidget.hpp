#pragma once
#include "Classes/Widget/UserWidget.hpp"
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"

class TestFps final : public UserWidget
{
	float per_delta;
    GCPtr<TextBlockWidget> text;

public:
    TestFps();

    void PreConstructEvent() override;
    void Tick(double delta_time) override;
};
