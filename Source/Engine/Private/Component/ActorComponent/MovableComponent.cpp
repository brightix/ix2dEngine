#include "../../../Classes/Component/ActorComponent/MovableComponent.hpp"
#include "Classes/Actor.hpp"
#include "Classes/Character.hpp"
#include "Classes/Pawn.hpp"
MovableComponent::MovableComponent() : base_move_speed(200)
{
	CNAME;
}


void MovableComponent::Construct()
{
    ActorComponent::Construct();
    if ((c = Cast<Character>(outer)))
    {
        EnhancedInputSubSystem* enhanced_input_sub_system = c->GetEnhancedInputSubSystem();
        enhanced_input_sub_system->AddInputEventBool(SDL_SCANCODE_W,[this](EnhancedInputParam<bool> eip) {
	        switch (auto f = eip.status)
            {
                case Triggered:
	                if (c->GetCharacterMoveStrategy() == God)
	                {
	                    player_input_Vec+=UpDir*1.f;
	                }
                    break;
                case EnhancedInputParamStatus::Start:
                    printf("%s\n",eip.input_action.key_name.c_str());
	                if (c->GetCharacterMoveStrategy() == Simulation)
	                {
	                    Jump();
	                }
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
            switch (eip.status)
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

void MovableComponent::Jump() const
{
    c->GetCharacterPhysicsBody()->AddImpulse({0,-100.f});
}

void MovableComponent::ActorComponentTick(const double deltaTime)
{
    const Vec2<float> movement = (player_input_Vec.Normalize() * deltaTime * base_move_speed).Cast<float>();
    owned_actor->AddActorWorldLocation(movement);
    player_input_Vec = {};
}
