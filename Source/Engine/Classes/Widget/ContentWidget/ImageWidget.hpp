#pragma once
#include "ContentWidget.hpp"

class ImageWidget : public ContentWidget
{
public:
    ImageWidget(){}
    void Construct() override
    {
        ContentWidget::Construct();
    }
    ~ImageWidget(){}
};
