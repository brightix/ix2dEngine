#pragma once
#include <string>

template<typename T>
class GCPtr;
struct GCObject
{
	bool bMarked = false;
	std::string name;
	std::vector<GCObject*> referenced;
	std::vector<GCObject*> referencing;


	//GC安全
	template<typename T, typename ...Args>
	GCPtr<T> make_GCPtr(Args&&...args)
	{
		return GCPtr<T>(new T(std::forward<Args>(args)...), this);
	}
	template<typename T, typename ...Args>
	GCPtr<T> make_GCPtr(T* p)
	{
		return GCPtr<T>(p, this);
	}
	virtual ~GCObject()= default;
};

