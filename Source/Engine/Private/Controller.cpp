#include "Classes/Controller.hpp"

#include <fstream>
#include <SDL3/SDL.h>

#include "Classes/Pawn.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include "../Utilities/ThirdParty/json.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../Classes/Core/GameEngine.hpp"
#include "System/Font.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

using namespace std;
using json = nlohmann::json;
Controller::Controller() : show_mouse_cursor(false)
{
	CNAME;
}

void Controller::Control(GCPtr<Pawn> pawn)
{
	controlled_pawn = pawn;
}

void Controller::Construct()
{
	Actor::Construct();

	SetHiddenInGame(true);
	input_map = NewObject<InputMap>(new InputMap());
	auto st = 	Root->MountedComponent(new StaticTexture);
	SetTextureFromSurface_S(st.Peek(),GetTextSurface("                    "));

	pawn_info = CreateWidget(new TextBlockWidget,this);
	auto p_slot = AddToViewport(pawn_info);
	p_slot->display_area.x = 400.f;

	pawn_physics_info = CreateWidget(new TextBlockWidget,this);
	auto p_phys_slot = AddToViewport(pawn_physics_info);
	p_phys_slot->display_area.x = 400.f;
	p_phys_slot->display_area.y = 24.f;
}

void Controller::Tick(double delta)
{
    //Actor::Tick(delta);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
    	auto t = event.type;

    	SDL_Scancode scancode = event.key.scancode;
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
	    			SDL_Scancode scancode = event.key.scancode;

	    			auto& key = keys_state[scancode];
	    			key = Complete;

	    			EnhancedInputParam<bool> eip(input_map->Enhanced[scancode],false,Complete);
	    			controlled_pawn->CallEnhancedInputEventBool(eip);
	    			key = Idle;
	    		}
        		break;
        	case SDL_EVENT_MOUSE_BUTTON_DOWN: // 鼠标按下
        		// event.button.button 获取按钮

        		break;

        	case SDL_EVENT_MOUSE_BUTTON_UP:   // 鼠标松开
        		break;

        	case SDL_EVENT_MOUSE_MOTION:     // 鼠标移动
        		// event.motion.x / y / xrel / yrel
	    		mouse_pos = {event.motion.x,event.motion.y};
        		break;

        	case SDL_EVENT_MOUSE_WHEEL:      // 鼠标滚轮
        		// event.wheel.x / y
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
	auto pawn_loc = controlled_pawn->GetSceneComponent("default_texture").Cast<StaticTexture>()->GetComponentWorldLocation();
	pawn_info->SetText(pawn_loc.str());

	auto physics_info = controlled_pawn->GetSceneComponent("default_texture").Cast<StaticTexture>()->GetPhysicsBody()->velocity;
	pawn_physics_info->SetText(physics_info.str());
}

Vec2<float> Controller::GetMousePos() const
{
	return mouse_pos;
}

GCWeakPtr<Pawn> Controller::GetControlledPawn() const
{
	return controlled_pawn;
}

Vec2<float> GetMousePos(GCWeakPtr<Controller> controller)
{
	return controller->GetMousePos();
}
