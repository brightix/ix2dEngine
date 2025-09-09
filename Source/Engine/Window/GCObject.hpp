#pragma once


struct GCObject
{
	bool bMarked = false;
	std::vector<GCObject*> referenced;
	std::vector<GCObject*> referencing;
	virtual ~GCObject()= default;
};

