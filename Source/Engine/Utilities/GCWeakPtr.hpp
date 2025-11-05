#pragma once
#include "GCPtr.hpp"
#include "Classes/Core/GCObject.hpp"
//
// template<typename T>
// struct GCPtr
// {
// 	T* ptr;
// 	size_t weak_id;
// 	GCPtr() : ptr(nullptr), weak_id(-1) { }
//
// 	GCPtr(T* obj)
// 	{
// 		static_assert(std::is_base_of_v<GCObject, T>,"使用Weak必须是继承自GCObject");
// 		ptr = obj;
// 		weak_id = ptr != nullptr ? obj->id : -1;
// 	}
// 	T* Peek() const
// 	{
// 		const auto it = Global_GCObject_Registry.find(weak_id);
// 		if (it != Global_GCObject_Registry.end() && !it->second->is_pending_kill)
// 			return ptr;
//
// 		return nullptr;
// 	}
// 	explicit operator bool() const noexcept
// 	{
// 		if (Global_GCObject_Registry.contains(weak_id) && !Global_GCObject_Registry[weak_id]->is_pending_kill)
// 		{
// 			return true;
// 		}
// 		return false;
// 	}
//
// 	bool IsValid() const
// 	{
// 		return Global_GCObject_Registry.contains(weak_id) && !Global_GCObject_Registry[weak_id]->is_pending_kill;
// 	}
//
// 	//不检查合法性
// 	T operator*()
// 	{
// 		return *ptr;
// 	}
// 	T* operator->()
// 	{
// 		return ptr;
// 	}
// 	bool operator==(const GCPtr& other) const
// 	{
// 		return ptr == other.ptr;
// 	}
// 	template<typename U>
// 	GCPtr(const GCPtr<U>& other) : weak_id(other.weak_id)
// 	{
// 		static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
// 					  "Types must be in same inheritance hierarchy");
// 		ptr = static_cast<T*>(other.ptr);
// 	}
//
// 	GCPtr(GCPtr<T>& other) : ptr(other.Get()), weak_id(other.id){}
//
// 	template<typename U>
// 	GCPtr(GCPtr<U>& other)
// 	{
// 		static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
// 			  "Types must be in same inheritance hierarchy");
// 		ptr = static_cast<T*>(other.Get());
// 		weak_id = other.id;
// 	}
//
// 	GCPtr(const GCPtr<T>& other) : ptr(other.Get()), weak_id(other.id)
// 	{
//
// 	}
//
// 	template<typename U>
// 	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), weak_id(other.id)
// 	{
// 		static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
// 			  "Types must be in same inheritance hierarchy");
// 	}
//
// 	template<typename U>
// 	GCPtr<U> Cast()
// 	{
// 		U* p = dynamic_cast<U*>(ptr);
// 		if (!p)
// 		{
// 			LogWithLevel("GCWeak Cast失败",FatalError);
// 		}
// 		return GCPtr<U>(p);
// 	}
//
// };

namespace std {
	template<typename T>
	struct hash<GCPtr<T>>
	{
		size_t operator()(const GCPtr<T>& w) const noexcept
		{
			return std::hash<T*>{}(w.ptr); // 使用 ptr 地址作为 hash
		}
	};
}