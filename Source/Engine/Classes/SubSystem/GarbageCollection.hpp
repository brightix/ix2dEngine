#pragma once
#include "Sub/EngineSubsystem.hpp"


struct GCObject;

class GarbageCollection : public EngineSubSystem
{
	//std::vector<GCObject*> GCAllObjects;
	int Block = 0;
public:
    GarbageCollection();
	void GCMark(GCObject *gc_object);
    int GCSweep();

};

