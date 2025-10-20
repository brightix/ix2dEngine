#pragma once
#include <iostream>

#include "Actor.hpp"
#include "InputMap.hpp"
#include "Types/Vec.hpp"
#include "../Utilities/FuncLib/GlobalMacros.hpp"
#include "Core/GameWorld.hpp"

enum EnhancedInputParamStatus : int;
class Object;
class Pawn;



using EnhancedInputParamVariant = std::variant<bool,float,Vec2<double>,Vec<double>>;

class Controller : public Actor
{
    GCWeakPtr<Pawn> controlled_pawn;
    //std::unordered_map<std::string,std::function<void()>>;
    std::unordered_map<std::string,EventMethod> enhancedInput;

    GCPtr<Object> CurrentFocus;
	GCPtr<InputMap> input_map;
	std::unordered_map<SDL_Scancode,EnhancedInputParamStatus> keys_state;
    //bool
    bool show_mouse_cursor;

	//Debug
	//GCPtr<StaticTexture> pawn_location_tex;

public:
    Controller();
    ~Controller() override = default;
	void Control(GCPtr<Pawn> pawn);
	void Construct() override;
    void Tick(double delta) override;

    GCWeakPtr<Pawn> GetControlledPawn() const;
};

static void BindNormalKeyEvent(Object* obj, const std::string& event_name, const Event &event)
{
	GetWorld()->GetController(0)->dispatcher_system.BindEventTo(obj, event_name, event);
}