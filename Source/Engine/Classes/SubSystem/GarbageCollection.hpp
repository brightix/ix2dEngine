#pragma once
#include "Sub/EngineSubsystem.hpp"


struct GCObject;

//std::vector<Ptr<Object>> GlobalPtr;
class GarbageCollection : public EngineSubSystem
{
	//std::vector<GCObject*> GCAllObjects;
	int Block = 0;
public:
    GarbageCollection();
	void GCMark(GCObject *gc_object);

	static void GCPtrMark();

	int GCSweep();
};

