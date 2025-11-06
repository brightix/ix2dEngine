#pragma once
#include <variant>

#include "Actor.hpp"
#include "InputMap.hpp"
#include "Types/Vec.hpp"
#include "Core/GameWorld.hpp"

enum EnhancedInputParamStatus : int;
class Object;
class Pawn;

using EnhancedInputParamVariant = std::variant<bool,float,Vec2<double>,Vec<double>>;

class Controller : public Actor
{
    GCPtr<Pawn> controlled_pawn;
    //std::unordered_map<std::string,std::function<void()>>;
    std::unordered_map<std::string,EventMethod> enhancedInput;

    GCPtr<Object> CurrentFocus;
	GCPtr<InputMap> input_map;
	//各种输入数据

	std::unordered_map<SDL_Scancode,EnhancedInputParamStatus> keys_state;

	Vec2<float> mouse_pos;
	std::unordered_map<SDL_Scancode,EnhancedInputParamStatus> mouse_state;

    //bool
    bool show_mouse_cursor;
    GCPtr<TextBlockWidget> pawn_info;
    GCPtr<TextBlockWidget> pawn_physics_info;

    //Debug
	//GCPtr<StaticTexture> pawn_location_tex;

	using Actor::Actor;
public:
	void Control(GCPtr<Pawn> pawn);
	void Construct() override;
	void EventBegin() override;
    void Tick(double delta) override;


	Vec2<float> GetMousePos() const;

	void ReadInputMap(InputMap *map);

    [[nodiscard]] Pawn* GetControlledPawn() const;

};

static void BindNormalKeyEvent(Object* obj, const std::string& event_name, const Event &event)
{
	World()->GetController(0)->dispatcher_system.BindEventTo(obj, event_name, event);
}

Vec2<float> GetMousePos();


/**
 *  This widget has no parent unless it is added to a viewport or to another widget.
 * @tparam T
 * @param widget
 * @param outer
 * @return Wrappered by GCPtr
 */
template<typename T>
T* CreateWidget(T* widget, GCObject* outer = nullptr)
{
	static_assert(std::is_base_of_v<Widget, T>,"T must be derived by Widget");
	if (outer)
	{
		widget->outer = outer;
		GCLink(outer,widget);
	}
	widget->PreConstructEvent();
	return widget;
}

template<typename T,typename ...Args>
T* CreateWidget(Controller* outer = nullptr,Args...args)
{

	static_assert(std::is_base_of_v<Widget, T>,"T must be derived by Widget");
	T* widget = new T(std::forward<Args>(args)...);
	if (outer)
	{
		widget->outer = outer;
	}
	else
	{
		//用户控件没有 PlayerController 默认添加进世界上下文
		widget->outer = World();
	}
#if DEBUG
	if (!outer)
	{
		Log("构造了野 控件 ");
	}
#endif
	widget->PreConstructEvent();
	return widget;
}