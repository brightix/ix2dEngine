#pragma once
#include "Classes/SubSystem/WorldSubSystem.hpp"


struct GCObject;

class GarbageCollection : public WorldSubSystem
{
	//std::vector<GCObject*> GCAllObjects;
public:
    GarbageCollection();
	void GCMark(GCObject *gc_object);
    int GCSweep();

    ~GarbageCollection() override = default;
};

