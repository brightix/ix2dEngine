#include "Classes/Core/GCObject.hpp"

#include "Utilities/GCPtr.hpp"

GCObject::GCObject()
{
	id = ++glo_id;
#if DEBUG
	if (id == 18)
	{
		//BREAK;
	}
#endif
	Global_GCObject_Registry[id] = this;
}
GCObject::~GCObject()
{
	Global_GCObject_Registry.erase(id);
}

void GCObject::NativeSetOuter(GCObject *new_outer)
{
	outer = new_outer;
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


// void GCObject::SetOuter(GCObject* new_outer)
// {
// 	outer = new_outer;
// }
