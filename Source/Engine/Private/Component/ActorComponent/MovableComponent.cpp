#include "../../../Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Actor.hpp"
#include "Classes/Pawn.hpp"
#include "Utilities/FuncLib/StaticCast.hpp"
MovableComponent::MovableComponent(): player_input_Vec(), base_move_speed(200) {}
MovableComponent::MovableComponent(Actor *owner): ActorComponent(owner), player_input_Vec(), base_move_speed(200)
{
}


void MovableComponent::Construct()
{
    ActorComponent::Construct();
    if (auto t = Cast<Pawn>(owner))
    {
        EnhancedInputSubSystem* enhanced_input_sub_system = t->GetEnhancedInputSubSystem();
        enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_W,[this](EnhancedInputParam<bool> eip) {
            auto f = eip.status;
            switch (f)
            {
                case Triggered:
                    player_input_Vec+=UpDir*1.f;
                    break;
                case EnhancedInputParamStatus::Start:
                    printf("%s\n",eip.input_action.key_name.c_str());
                    break;
                case Cancel:
                    break;
                case Complete:
                    break;
                default: break;
            }
        });
        enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_S,[this](EnhancedInputParam<bool> eip) {
            auto f = eip.status;
            switch (f)
            {
                case Triggered:
                    player_input_Vec+=DownDir*1.f;
                    break;
                case EnhancedInputParamStatus::Start:
                    printf("%s\n",eip.input_action.key_name.c_str());
                    break;
                case Cancel:
                    break;
                case Complete:
                    printf("%s %s\n",eip.input_action.key_name.c_str(),"Complete");
                    break;
                default: break;
            }
        });
        enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_A,[this](EnhancedInputParam<bool> eip) {
            auto f = eip.status;
            switch (f)
            {
                case Triggered:
                    player_input_Vec+=LeftDir*1.f;
                    break;
                case EnhancedInputParamStatus::Start:
                    printf("%s\n",eip.input_action.key_name.c_str());
                    break;
                case Cancel:
                    break;
                case Complete:
                    break;
                default: break;
            }
        });
        enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_D,[this](EnhancedInputParam<bool> eip) {
            auto f = eip.status;
            switch (f)
            {
                case Triggered:
                    player_input_Vec+=RightDir*1.f;
                    break;
                case EnhancedInputParamStatus::Start:
                    printf("%s\n",eip.input_action.key_name.c_str());
                    break;
                case Cancel:
                    break;
                case Complete:
                    break;
                default: break;
            }
        });
    }
    std::cout<< "增强输入子系统初始化完毕" << std::endl;
}

void MovableComponent::ActorComponentTick(double deltaTime)
{
    owner->AddActorWorldLocation(player_input_Vec.Normalize() * static_cast<float>(deltaTime) * base_move_speed);
    player_input_Vec = {};
}
