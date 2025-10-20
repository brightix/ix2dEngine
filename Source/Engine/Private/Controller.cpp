#include "Classes/Controller.hpp"

#include <fstream>
#include <SDL3/SDL.h>

#include "Classes/Pawn.hpp"
#include "Classes/SubSystem/EnhancedInputSubSystem.hpp"
#include "Utilities/json.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "../Classes/Core/GameEngine.hpp"
#include "System/Font.hpp"
#include "Classes/Component/SenceComponent/StaticTexture.hpp"

using namespace std;
using json = nlohmann::json;
Controller::Controller() : show_mouse_cursor(false)
{
	//controlled_pawn = SpawnActorFromSelf<Pawn>(new Pawn());
	//input_map = make_GCPtr<InputMap>();
}

void Controller::Control(GCPtr<Pawn> pawn)
{
	controlled_pawn = pawn;
}

void Controller::Construct()
{
	Actor::Construct();

	input_map = NewObject<InputMap>(new InputMap());
	auto st = NewObject<StaticTexture>(new StaticTexture);
	st->AsyncSetTextureFromSurface(FontRenderer::GetTextSurface("                    "));
	Root->MountedComponent(st);

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
        		GameEngine::Instance().Quit();
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

	// if (controlled_pawn)
	// {
	// 	auto task = RenderTask();
	// 	task.task = [this](SDL_Renderer* r) {
	// 		SDL_RenderTexture(pawn_location_tex->GetTexture(),controlled_pawn->GetWorldLocation().str());
	// 		//FontRenderer::Instance().UpdateTextTexture(pawn_location_tex.Get(),controlled_pawn->GetWorldLocation().str());
	//
	// 		auto dst = SDL_FRect(0,200,pawn_location_tex->w,pawn_location_tex->h);
	// 		SDL_RenderTexture(r, pawn_location_tex->GetTexture(), nullptr, &dst);
	// 	};
	// }
	// 	ToRender(task);
}

GCWeakPtr<Pawn> Controller::GetControlledPawn() const
{
	return controlled_pawn;
}
