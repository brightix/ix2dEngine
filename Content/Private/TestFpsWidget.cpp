#include "../Public/TestFpsWidget.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Widget/ContentWidget/TextBlockWidget.hpp"
#include "System/Font.hpp"

TestFps::TestFps() : per_delta(0)
{
}


void TestFps::PreConstructEvent()
{
    UserWidget::PreConstructEvent();
	CNAME;
    auto text = CreateWidget<TextBlockWidget>();
    FontStyle fontStyle;
    fontStyle.font = FontRenderer::Instance().GetFont("simkai", 24);
    try{
    	text->SetText("Hello World!");
		text->SetFontStyle(fontStyle);
    }
	catch (...)
	{
		SDL_GetError();
	}
    AddChild("Fps_Text", text);

    //Root->widget = text;
}

void TestFps::Tick(double delta_time)
{
	per_delta += delta_time;
	if (per_delta > 0.15f)
	{
		per_delta = 0.f;
		auto info = GameEngine::Instance().GetEngineAttribution();
		GetChild("Fps_Text").Cast<TextBlockWidget>()->SetText(ix::to_string(1.0/info.DeltaTime,0));
	}
}
