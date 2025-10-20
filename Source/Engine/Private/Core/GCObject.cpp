#include "Classes/Core/GCObject.hpp"

#include "Utilities/GCPtr.hpp"

GCObject::GCObject()
{
	//printf("s\n");
	id = glo_id++;
	Global_GCObject_Registry[id] = this;
}
