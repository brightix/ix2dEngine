#include "Classes/Core/GCObject.hpp"

#include "Utilities/GCPtr.hpp"

GCObject::GCObject()
{
	//printf("s\n");
	id = glo_id++;
	if (id == 18)
	{
		BREAK;
	}
	Global_GCObject_Registry[id] = this;
}
GCObject::~GCObject()
{
	Global_GCObject_Registry.erase(id);
}

void GCObject::NativeSetOuter(GCObject *new_outer)
{
	GCUnLink(outer,this);
	outer =	new_outer;
	GCLink(outer,this);
}

bool GCObject::IsActive() const
{
	return Global_GCObject_Registry.contains(id) && !Global_GCObject_Registry[id]->is_pending_kill;
}

std::string GCObject::GetClassName()
{
	return class_name;
}

void GCObject::GCUnlink_self()
{
	for (auto parent : referenced)
	{
		parent->referencing.remove(this);
	}
	for (auto child : referencing)
	{
		child->referenced.remove(this);
	}
}

void GCLink(GCObject *parent, GCObject *child)
{
	if (!child || !parent)
	{
		Log("GCLink 绑定到空指针");
		return ;
	}
	parent->referencing.insert(child);
	child->referenced.insert(parent);
}

void GCUnLink(GCObject *parent, GCObject *child)
{
	if (!child || !parent)
	{
		Log("GCUnLink 解绑定到空指针");
		return ;
	}

	parent->referencing.remove(child);
	child->referenced.remove(parent);
}


// void GCObject::SetOuter(GCObject* new_outer)
// {
// 	outer = new_outer;
// }
