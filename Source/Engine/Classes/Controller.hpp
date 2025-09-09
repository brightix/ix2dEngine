#pragma once
#include <iostream>

#include "Actor.h"
#include "InputMap.hpp"
#include "Types/Vec.hpp"
#include "Window/GlobalMacros.hpp"

class Object;
class Pawn;



using EnhancedInputParamVariant = std::variant<bool,float,Vec2d<double>,Vec<double>>;

class Controller : public Actor
{
    GCPtr<Pawn> controlled_pawn;
    //std::unordered_map<std::string,std::function<void()>>;
    std::unordered_map<std::string,EventMethod> enhancedInput;

    GCPtr<Object> CurrentFocus;
	GCPtr<InputMap> input_map;
    //bool
    bool show_mouse_cursor;
public:
    Controller();
    ~Controller()= default;

    void Tick(double delta) override;
};

