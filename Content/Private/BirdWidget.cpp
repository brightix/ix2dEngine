#include "Public/BirdWidget.hpp"

#include "Classes/Controller.hpp"
#include "Classes/Widget/Slot/CanvasSlot.hpp"
#include "Public/GroundManager.hpp"

void BirdWidget::RegisterEvents()
{
    UserWidget::RegisterEvents();
}

void BirdWidget::PreConstructEvent()
{
    UserWidget::PreConstructEvent();
    auto canvas = AddChild(CreateWidget<CanvasWidget>());
    score_widget = CreateWidget<TextBlockWidget>();
    auto slot = canvas->widget->AddChild(score_widget);
    slot->display_area = {0,500,500,500};
}

void BirdWidget::ConstructEvent()
{
    UserWidget::ConstructEvent();
    auto manager = GetAllActorFromClass<GroundManager>();
    ListenDispatcher(manager[0],"OnAddScore",&BirdWidget::SetScore);
}

void BirdWidget::Tick(double delta_time)
{
    UserWidget::Tick(delta_time);
}

void BirdWidget::SetScore(int a)
{
	score_widget->SetText("得分：" + std::to_string(a));
}



