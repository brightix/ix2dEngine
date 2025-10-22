#pragma once
#include "Classes/Widget/ContentWidget/ImageWidget.hpp"

class TestImageWidget : public ImageWidget
{
public:
    TestImageWidget(){}
    void Construct() override
    {
        ImageWidget::Construct();
    }
    ~TestImageWidget(){}
};
