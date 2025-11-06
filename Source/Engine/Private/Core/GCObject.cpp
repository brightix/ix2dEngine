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

// void GCObject::SetOuter(GCObject* new_outer)
// {
// 	outer = new_outer;
// }
