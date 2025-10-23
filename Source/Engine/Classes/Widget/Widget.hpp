#pragma once
#include "Classes/Object.hpp"
#include "Types/FRect.hpp"

enum class WidgetVisibility;

class Widget : public Object
{
    //std::unordered_set<size_t> children_ids;
protected:
	WidgetVisibility widget_visibility;
	int layer_id;
	GCWeakPtr<Widget> parent;
public:
	bool dirty;
    Widget();

	void Construct() override;
	virtual void Tick(double delta_time){}



	//渲染
	virtual void flush(){}
	virtual void WidgetRender(FRect display_area)= 0;


	virtual void AddChild(GCPtr<Widget> UI){}
	WidgetVisibility GetVisibility();
	void SetVisibility(WidgetVisibility new_Visibility);

	virtual void WidgetEventBegin(){}



	void RemoveFromParent();
	virtual void RemoveChild(Widget* UI){}

	int GetLayerId() const;

    void MakeDirty();

	Widget(const Widget&) = default;             // 显式允许拷贝
	Widget(Widget&&) noexcept = default;         // 显式允许移动
	~Widget() override = default;
};

