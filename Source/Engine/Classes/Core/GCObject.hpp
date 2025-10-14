#pragma once
#include <string>
#include <vector>

static int glo_id;
template<typename T>
class GCPtr;
struct GCObject
{
	bool bMarked = false;
	bool is_pending_kill = false;
	int id;
	std::vector<GCObject*> referenced;
	std::vector<GCObject*> referencing;
	GCObject()
	{
		//printf("s\n");
		id = glo_id++;
	}

	//GC安全
	template<typename T, typename ...Args>
	GCPtr<T> make_GCPtr(Args&&...args)
	{
		return GCPtr<T>(new T(std::forward<Args>(args)...), this);
	}
	template<typename T>
	GCPtr<T> make_GCPtr(T* p)
	{
		return GCPtr<T>(p, this);
	}
	virtual ~GCObject()
	{
	}
};

