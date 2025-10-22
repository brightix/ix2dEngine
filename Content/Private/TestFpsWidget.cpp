#include "../Public/TestFpsWidget.hpp"

#include "System/Font.hpp"

TestFpsWidget::TestFpsWidget()
{
}

void TestFpsWidget::Construct()
{
    TextBlockWidget::Construct();
    widget_texture = NewObject(new StaticTexture);
    FontStyle fontStyle;
    fontStyle.font = FontRenderer::Instance().GetFont("arial.ttf", 24);
    SetFontStyle(fontStyle);
    SetText("Hello World!");
}
