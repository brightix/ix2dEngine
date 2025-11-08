#pragma once
#include "GCPtr.hpp"
#include "Classes/Core/GCObject.hpp"


//可以使用安全的Peek访问指针，但不会延长ptr的生命周期
template<typename T>
struct GCWeakPtr
{
	T* ptr;
	size_t weak_id;
	GCWeakPtr() : ptr(nullptr), weak_id(0) { }

	GCWeakPtr(T* obj)
	{
		static_assert(std::is_base_of_v<GCObject, T>,"使用Weak必须是继承自GCObject");
		ptr = obj;
		weak_id = ptr != nullptr ? obj->id : 0;
	}
	GCWeakPtr(const T* obj)
	{
		static_assert(std::is_base_of_v<GCObject, T>,"使用Weak必须是继承自GCObject");
		ptr = obj;
		weak_id = ptr != nullptr ? obj->id : 0;
	}
	T* Peek() const
	{
		const auto it = Global_GCObject_Registry.find(weak_id);
		if (it != Global_GCObject_Registry.end() && !it->second->is_pending_kill)
			return ptr;
		return nullptr;
	}
	explicit operator bool() const noexcept
	{
		if (Global_GCObject_Registry.contains(weak_id) && !Global_GCObject_Registry[weak_id]->is_pending_kill)
		{
			return true;
		}
		return false;
	}

	bool IsValid() const
	{
		return Global_GCObject_Registry.contains(weak_id) && !Global_GCObject_Registry[weak_id]->is_pending_kill;
	}

	//不检查合法性
	T operator*()
	{
		return *ptr;
	}
	T* operator->()
	{
		return ptr;
	}
	bool operator==(const GCWeakPtr& other) const
	{
		return ptr == other.ptr;
	}
	template<typename U>
	GCWeakPtr(const GCWeakPtr<U>& other) : weak_id(other.weak_id)
	{
		static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
					  "Types must be in same inheritance hierarchy");
		ptr = static_cast<T*>(other.ptr);
	}

	GCWeakPtr(GCWeakPtr<T>& other) : ptr(other.ptr), weak_id(other.weak_id){}

	template<typename U>
	GCWeakPtr(GCWeakPtr<U>& other)
	{
		static_assert(std::is_base_of_v<T, U> || std::is_base_of_v<U, T>,
			  "Types must be in same inheritance hierarchy");
		ptr = static_cast<T*>(other.Get());
		weak_id = other.id;
	}

	GCWeakPtr(const GCWeakPtr<T>& other) : ptr(other.ptr), weak_id(other.weak_id)
	{}

	template<typename U>
	GCWeakPtr<U> Cast()
	{
		U* p = dynamic_cast<U*>(ptr);
		if (!p)
		{
			LogWithLevel(FatalError,"GCWeak Cast失败");
		}
		return GCWeakPtr<U>(p);
	}
	GCObject* Get() const noexcept
	{
		return ptr;
	}
};

template<typename T>
struct std::hash<GCWeakPtr<T>>
{
	size_t operator()(const GCWeakPtr<T>& w) const noexcept
	{
		return std::hash<T*>{}(w.ptr); // 使用 ptr 地址作为 hash
	}
};
