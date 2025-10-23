#include "../Public/TestFpsWidget.hpp"

#include "Classes/Core/GameEngine.hpp"
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

void TestFpsWidget::Tick(double delta_time)
{
    TextBlockWidget::Tick(delta_time);
    auto info = GameEngine::Instance().GetEngineAttribution();
    SetText(std::to_string(1.0/info.DeltaTime));
}
