#pragma once
#include <unordered_set>
#include "Classes/Object.hpp"
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
	virtual void AddChild(GCPtr<Widget> UI){}
	WidgetVisibility GetVisibility();
	void SetVisibility(WidgetVisibility new_Visibility);

	void RemoveFromParent();
	void RemoveChild(Widget* UI);


	int GetLayerId() const;

	// template<typename T>
	// GCPtr<T> CreateWidget(T* new_widget)
	// {
	// 	return GCPtr<>
	// }
	virtual void flush(){}
	virtual void WidgetRender()= 0;
};

struct WidgetIDHash
{
	size_t operator()(const Widget& key) const noexcept
	{
		return std::hash<std::string>()(key.name) ^ (std::hash<size_t>()(key.id) << 1);
	}
};

