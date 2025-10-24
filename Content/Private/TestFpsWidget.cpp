#include "../Public/TestFpsWidget.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"
#include "System/Font.hpp"

TestFps::TestFps()
{
}



void TestFps::PreConstructEvent()
{
    UserWidget::PreConstructEvent();

    auto text = CreateWidget(new TextBlockWidget);
    FontStyle fontStyle;
    fontStyle.font = FontRenderer::Instance().GetFont("arial.ttf", 24);
    text->SetFontStyle(fontStyle);
    text->SetText("Hello World!");
    AddChild("Fps_Text", text);

    Root->widget = text;
}

void TestFps::Tick(double delta_time)
{
    auto info = GameEngine::Instance().GetEngineAttribution();
    GetChild("Fps_Text").Cast<TextBlockWidget>()->SetText(std::to_string(1.0/info.DeltaTime));
}
