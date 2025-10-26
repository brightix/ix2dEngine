#pragma once
#include "Classes/Object.hpp"
#include "Slot/PanelSlot.hpp"
#include "Types/FRect.hpp"
#include "Classes/Widget/Slot/PanelSlot.hpp"

enum class WidgetVisibility;
enum class WidgetType
{
	None,
	PanelWidget,
	ContentWidget,
	UserWidget
};

class Widget : public Object
{
    //std::unordered_set<size_t> children_ids;
protected:
	WidgetVisibility widget_visibility;
	int layer_id;
	GCWeakPtr<Widget> parent;
	GCPtr<PanelSlot> Root;
public:
	bool dirty;
	bool is_initialized;
    Widget();

	void Construct() final {}
	virtual void PreConstructEvent()
	{
		Root = NewObject(new PanelSlot);
	}
	virtual void ConstructEvent(){}

	virtual void ForTick(double delta_time);

	//渲染
	virtual void flush(){}
	virtual void WidgetRender(FRect display_area){}
	//递归调用
	//系统调用的render
    virtual void NativeWidgetRender(FRect display_area);

	virtual void AddChild(GCPtr<Widget> child);
	WidgetVisibility GetVisibility();
	void SetVisibility(WidgetVisibility new_Visibility);

	virtual void WidgetEventBegin(){}



	void RemoveFromParent();
	virtual void RemoveChild(Widget* UI){}
	virtual std::vector<GCWeakPtr<PanelSlot>> GetChildren(){ return {}; }


	int GetLayerId() const;

    void MakeDirty();

    virtual void Tick(double delta_time){}


	//Widget类型
	virtual WidgetType GetWidgetType(){ return WidgetType::None; }
	Widget(const Widget&) = default;             // 显式允许拷贝
	Widget(Widget&&) noexcept = default;         // 显式允许移动
	~Widget() override = default;
};

/**
 *  This widget has no parent unless it is added to a viewport or to another widget.
 * @tparam T
 * @param widget
 * @return Wrappered by GCPtr
 */
template<typename T>
GCPtr<T> CreateWidget(T* widget)
{
	static_assert(std::is_base_of_v<Widget, T>,"T must be derived by Widget");
	auto it = GCPtr<T>(widget,nullptr);
	widget->PreConstructEvent();
	return it;
}

