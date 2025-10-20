#pragma once
#include <string>
#include <vector>
static size_t glo_id;
template<typename T>
class GCPtr;
struct GCObject
{
    std::string name = "未命名";
	bool bMarked = false;
	bool is_pending_kill = false;
	size_t id;
	std::vector<GCObject*> referenced;
	std::vector<GCObject*> referencing;
	GCObject();

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


	virtual ~GCObject()= default;
};

