#pragma once
#include <string>
#include <vector>
//
// #include "Classes/Object.hpp"
// template<typename T>
// class GCPtr;
// struct GCObject
// {
//     std::string class_name = "UnknownClass";
//     std::string name;
//
// 	bool bMarked = false;
// 	bool is_pending_kill = false;
// 	size_t id;
// 	std::vector<Object*> referenced;
// 	std::vector<Object*> referencing;
// 	GCObject();
//
//     virtual ~GCObject();
//
//
//     bool IsActive() const;
// 	std::string GetClassName();
// 	void SetName(const std::string& new_name);
//
//     //GC安全 只创建实例，不执行construct和绑定事件
// 	template<typename T, typename ...Args>
// 	GCPtr<T> NewGCPtr(Args&&...args)
// 	{
// 		return GCPtr<T>(new T(std::forward<Args>(args)...), this);
// 	}
// 	template<typename T>
// 	GCPtr<T> NewGCPtr(T* p)
// 	{
// 		return GCPtr<T>(p, this);
// 	}
//
// 	void GCUnlink()
// 	{
// 		for (auto parent : referenced)
// 		{
// 			std::erase(parent->referencing,this);
// 		}
// 		for (auto child : referencing)
// 		{
// 			std::erase(child->referenced,this);
// 		}
// 	}
// };

