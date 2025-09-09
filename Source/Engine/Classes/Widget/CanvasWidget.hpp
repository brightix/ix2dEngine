#pragma once
#include "Widget.hpp"
#include "Types/Vec.hpp"


class CanvasWidget : public Widget
{
	Vec2d<float> Size;
public:

    CanvasWidget();
    ~CanvasWidget()= default;
};

