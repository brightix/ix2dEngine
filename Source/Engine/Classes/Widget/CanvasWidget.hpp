#pragma once
#include "Widget.hpp"
#include "Types/Vec.hpp"


class CanvasWidget : public Widget
{
	Vec2<float> Size;
public:

    CanvasWidget();
    ~CanvasWidget()= default;
};

