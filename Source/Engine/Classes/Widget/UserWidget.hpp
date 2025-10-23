#pragma once
#include "Widget.hpp"


class UserWidget : public Widget
{
    GCPtr<Widget> child_widget;
public:
    UserWidget();
    ~UserWidget()= default;

};

