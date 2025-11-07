#include "Classes/Controller.hpp"

#include <SDL3/SDL.h>

#include "Classes/Pawn.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include "Utilities/ThirdParty/json.hpp"
#include "Classes/Core/GameEngine.hpp"
#include "Classes/Component/SenceComponent/StaticTextureComponent.hpp"

using namespace std;
using json = nlohmann::json;

void Controller::Possess(Pawn* pawn)
{
	if (controlled_pawn)
	{
		//解绑
		CallEvent("OnUnPossess");
	}
	controlled_pawn = pawn;
	CallEvent("OnControlled");
}

void Controller::UnPossess(Pawn* pawn)
{
	if (pawn->)
	CallEvent("OnDecontrol");

}


void Controller::Construct()
{
	Actor::Construct();
	CNAME;
	SetHiddenInGame(true);
	input_map = NewObject<InputMap>(this);
	ReadInputMap(input_map.Get());

	pawn_info = CreateWidget<TextBlockWidget>(this);
	auto p_slot = AddToViewport(pawn_info.Get());
	p_slot->display_area.x = 400.f;

	pawn_physics_info = CreateWidget<TextBlockWidget>(this);
	auto p_phys_slot = AddToViewport(pawn_physics_info.Get());
	p_phys_slot->display_area.x = 400.f;
	p_phys_slot->display_area.y = 24.f;
}

void Controller::EventBegin()
{
	Actor::EventBegin();
}

void Controller::Tick(double delta)
{
    //Actor::Tick(delta);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
    	auto t = event.type;

    	SDL_Scancode scancode = event.key.scancode;
    	EventParams e;

    	Uint8 button_id;
	    switch (t)
        {
	        case SDL_EVENT_QUIT:
        		// 用户点击关闭按钮
        		//running = false;
        		GameEngine::Instance().Stop();
        		return;

        	case SDL_EVENT_KEY_DOWN: // 键盘按下
        		if (controlled_pawn)
        		{
					auto& key = keys_state[scancode];
        			if (key == Idle)
        			{
        				// Start
        				key = Start;
        				//cout << "start" << endl;
        				EnhancedInputParam<bool> eip(input_map->Enhanced[scancode],true,key);
        				controlled_pawn->CallEnhancedInputEventBool(eip);
        			}
        		}
	    		dispatcher_system.CallDispatcher(input_map->Normal[scancode].key_name);
        		break;
        	case SDL_EVENT_KEY_UP:   // 键盘松开
	    		if (controlled_pawn)
	    		{
	    			// Completed

	    			auto& key = keys_state[scancode];
	    			key = Complete;

	    			EnhancedInputParam<bool> eip(input_map->Enhanced[scancode],false,Complete);
	    			controlled_pawn->CallEnhancedInputEventBool(eip);
	    			key = Idle;
	    		}
        		break;
        	case SDL_EVENT_MOUSE_BUTTON_DOWN: // 鼠标按下
        		// event.button.button 获取按钮
	    		button_id = event.button.button;
	    		e.Add("pressed",true);
	    		dispatcher_system.CallDispatcher(input_map->Mouse[button_id].button_name,e);
        		break;

        	case SDL_EVENT_MOUSE_BUTTON_UP:   // 鼠标松开
	    		button_id = event.button.button;
	    		e.Add("pressed",false);
	    		dispatcher_system.CallDispatcher(input_map->Mouse[button_id].button_name,e);
        		break;
        	case SDL_EVENT_MOUSE_MOTION:     // 鼠标移动
        		// event.motion.x / y / xrel / yrel
	    		mouse_pos = {event.motion.x,event.motion.y};
	    		e.Add("mouse_pos",Vec2<float>(mouse_pos.x,mouse_pos.y));
	    		//dispatcher_system.CallDispatcher(input_map->Mouse[scancode].key_name,e);
        		break;

        	case SDL_EVENT_MOUSE_WHEEL:      // 鼠标滚轮
        		// event.wheel.x / y
	    		e.Add("mouse_wheel",event.wheel.y);
	    		dispatcher_system.CallDispatcher(input_map->Normal[scancode].key_name,e);
        		break;
        	default:
        		break;
        }
	}

	const bool* state = SDL_GetKeyboardState(nullptr);
	for (auto& x : keys_state)
	{
		if (state[x.first])
		{
			if (x.second == Start)
			{
				x.second = Triggered;
				continue;
			}
			//cout << "triggered" << endl;
			EnhancedInputParam<bool> eip(input_map->Enhanced[x.first],false,Triggered);
			controlled_pawn->CallEnhancedInputEventBool(eip);
		}
	}
	pawn_info->SetText(controlled_pawn->GetWorldTransform().location.str());
	pawn_physics_info->SetText(controlled_pawn->GetSceneComponent("capsule")->GetPhysicsBody()->velocity.str());
}

Vec2<float> Controller::GetMousePos() const
{
	return mouse_pos;
}

void Controller::ReadInputMap(InputMap* map)
{

	for (auto &key: map->Normal | views::values)
	{
		dispatcher_system.AddEventDispatcher(key.key_name);
	}
}

Pawn* Controller::GetControlledPawn() const
{
	return controlled_pawn.Get();
}

Vec2<float> GetMousePos(const Controller* controller)
{
	return controller->GetMousePos();
}
