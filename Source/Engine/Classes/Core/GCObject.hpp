#pragma once
#include <string>
#include <vector>

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
static size_t glo_id = 1;
template<typename T>
class GCPtr;
struct GCObject
{
    std::string class_name = "UnknownClass";
    std::string name;

	bool bMarked = false;
	bool is_pending_kill = false;
	size_t id;
	std::vector<GCObject*> referenced;
	std::vector<GCObject*> referencing;


	GCObject* outer;
	GCObject();

    virtual ~GCObject();


    bool IsActive() const;
	std::string GetClassName();
	template<typename T>
	void SetOuter(T* new_outer)
	{
		static_assert(std::is_base_of_v<GCObject,T>);
		outer = static_cast<GCObject*>(new_outer);
	}

	void GCUnlink_self()
	{
		for (auto parent : referenced)
		{
			std::erase(parent->referencing,this);
		}
		for (auto child : referencing)
		{
			std::erase(child->referenced,this);
		}
	}
};

inline void GCLink(GCObject* parent,GCObject* child)
{
	if (!child || !parent)
	{
		Log("GCLink 绑定到空指针");
		return ;
	}
	parent->referencing.push_back(child);
	child->referenced.push_back(parent);
}
inline void GCUnLink(GCObject* parent,GCObject* child)
{
	if (!child || !parent)
	{
		Log("GCUnLink 解绑定到空指针");
		return ;
	}
	std::erase(parent->referencing,child);
	std::erase(child->referenced,parent);
}

