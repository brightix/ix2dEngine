#include "Classes/Core/GCObject.hpp"

#include "Utilities/GCPtr.hpp"

GCObject::GCObject()
{
	//printf("s\n");
	id = glo_id++;
	Global_GCObject_Registry[id] = this;
}
GCObject::~GCObject()
{
	Global_GCObject_Registry.erase(id);
}

bool GCObject::IsActive() const
{
	return Global_GCObject_Registry.contains(id) && !Global_GCObject_Registry[id]->is_pending_kill;
}

std::string GCObject::GetClassName()
{
	return class_name;
}
