#pragma once
#include "Classes/Object.hpp"
#include "Slot/PanelSlot.hpp"
#include "Types/FRect.hpp"
#include "Types/RenderData.hpp"
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
	//每一个控件都拥有父控件，通过 AddChild() 获得
	GCPtr<Widget> parent_widget;
	GCPtr<PanelSlot> parent_slot;
	std::vector<GCPtr<PanelSlot>> slots;
public:
	int layer_id;
	bool dirty;
	bool is_initialized;
    Widget();

	void Construct() final ;
	virtual void PreConstructEvent(){}
	virtual void ConstructEvent(){}

	virtual void ForTick(double delta_time);

	//渲染
	virtual void flush(){}
	virtual void WidgetRender(FRect display_area){}
	//递归调用
	//系统调用的render
    virtual void NativeWidgetRender(FRect display_area);

    void NativeOfferRenderData(std::vector<RenderData> &data);
	virtual void OfferWidgetRenderData(std::vector<RenderData>& data){}


    virtual PanelSlot* AddChild(Widget *child);
	virtual PanelSlot* CreateSlot()= 0;
	virtual void ReceiveSlot(PanelSlot* slot)= 0;
	WidgetVisibility GetVisibility() const;
	void SetVisibility(WidgetVisibility new_Visibility);

	virtual void WidgetEventBegin(){}



	void RemoveFromParent();
	virtual void RemoveChild(Widget* UI){}

	int GetLayerId() const;

	//Slot
	PanelSlot* GetParentSlot() const;
	virtual std::vector<PanelSlot*> GetSlots() const;


	void MakeDirty();
	//刷新逻辑位置
	virtual void FlushDirty()= 0;


    virtual void Tick(double delta_time){}


	//Widget类型
	virtual WidgetType GetWidgetType(){ return WidgetType::None; }
	Widget(const Widget&) = default;             // 显式允许拷贝
	Widget(Widget&&) noexcept = default;         // 显式允许移动
	~Widget() override = default;
};

