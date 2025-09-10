#include "Classes/Controller.hpp"

#include <fstream>
#include <SDL3/SDL.h>

#include "Classes/Pawn.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include "Utilities/json.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../System/GameEngine.hpp"

using namespace std;
using json = nlohmann::json;
Controller::Controller() : show_mouse_cursor(false)
{
	controlled_pawn = make_GCPtr<Pawn>();
	input_map = make_GCPtr<InputMap>();
}

void Controller::Tick(double delta)
{
    //Actor::Tick(delta);
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
    	auto t = event.type;
	    switch (t)
        {
	        case SDL_EVENT_QUIT:
        		// 用户点击关闭按钮
        		//running = false;
        		GameEngine::Instance().Quit();
        		return;

        	case SDL_EVENT_KEY_DOWN: // 键盘按下
        		if (controlled_pawn)
        		{
			        SDL_Scancode scancode = event.key.scancode;

        			if (event.key.repeat == 0)
        			{
        				// Start
        				EnhancedInputParam<bool> eip(input_map->key_map[scancode],true,Start);
        				controlled_pawn->CallEnhancedInputEventBool(eip);
        			}
        			else
        			{
        				// Triggered
        				EnhancedInputParam<bool> eip(input_map->key_map[scancode],true,Triggered);
        				controlled_pawn->CallEnhancedInputEventBool(eip);
        			}
        		}
        		break;
        	case SDL_EVENT_KEY_UP:   // 键盘松开
	    		if (controlled_pawn)
	    		{
	    			// Completed
	    			SDL_Scancode scancode = event.key.scancode;

	    			EnhancedInputParam<bool> eip(input_map->key_map[scancode],false,Complete);
	    			controlled_pawn->CallEnhancedInputEventBool(eip);
	    		}
        		break;
        	case SDL_EVENT_MOUSE_BUTTON_DOWN: // 鼠标按下
        		// event.button.button 获取按钮
        		break;

        	case SDL_EVENT_MOUSE_BUTTON_UP:   // 鼠标松开
        		break;

        	case SDL_EVENT_MOUSE_MOTION:     // 鼠标移动
        		// event.motion.x / y / xrel / yrel
        		break;

        	case SDL_EVENT_MOUSE_WHEEL:      // 鼠标滚轮
        		// event.wheel.x / y
        		break;
        	default:
        		break;
        }
	}
}
