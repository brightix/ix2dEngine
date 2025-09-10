#pragma once
#include "System/GCObject.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include <cxxabi.h>
inline std::vector<GCObject*> GCAllObjects;

template<typename T>
class GCPtr
{
	T* ptr;
	GCObject* outer;
	// 构造
public:
	GCPtr() : ptr(nullptr), outer(nullptr) {}
	//新对象
	explicit GCPtr(T* p, GCObject* outer) : ptr(p), outer(outer)
	{
		static_assert(std::is_base_of_v<GCObject, T>, "T 必须继承自 GCObject");
		if(!outer)
		{
			Log("构造了野指针");
		}
		GCLink(ptr,outer);
		GCAllObjects.push_back(ptr);
	}
	//获取新引用
	explicit GCPtr(T* p) : ptr(p), outer(nullptr) {}

	//派生->基类
	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(nullptr) {}

	//给outer添加新对象引用
	GCPtr(const GCPtr& other, GCObject* outer)
	{
		ptr = other.ptr;
		this->outer = outer;
		GCLink(ptr,outer);
	}
	GCPtr(const GCPtr& other) : ptr(nullptr), outer(nullptr)
	{
		GCUnLink(ptr, outer);
		ptr = other.Get();
	}

	GCPtr& operator=(const GCPtr& other)
	{
		GCUnLink(ptr,outer);
		ptr = other.ptr;
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr& operator=(const GCPtr<U>& other) {
		GCUnLink(ptr,outer);
		ptr = other.Get();
		outer = nullptr;
		return *this;
	}
	// 移动赋值
	GCPtr& operator=(GCPtr&& other) noexcept
	{
		if (this != &other)
		{
			GCUnLink(ptr,outer);
			ptr = other.ptr;
			outer = other.outer;
			GCLink(ptr,outer);
		}
		return *this;
	}
	//移动构造 GC不安全
	GCPtr(GCPtr&& other) noexcept : ptr(other.ptr) ,outer(nullptr) {}

	void SetOwner(GCObject* owner)
	{
		outer = owner;
		GCLink(ptr,owner);
	}
	//assets
	~GCPtr() {}
	T* Get() const { return ptr; }
	GCObject* GetOuter() const { return outer; }
	T* operator->() const
	{
		return ptr;
	}
	T operator*() const
	{
		return *ptr;
	}
	explicit operator bool() const noexcept
	{
		return ptr != nullptr;
	}
//绑定GC关系
	static void GCLink(GCObject* child, GCObject* parent)
	{
		if (!child || !parent)
		{
			Log("GCLink 绑定到空指针");
			return ;
		}
		child->referenced.push_back(parent);
		parent->referencing.push_back(child);
	}
	static void GCUnLink(GCObject* child, GCObject* parent)
	{
		if (!child || !parent)
		{
			Log("GCLink 绑定到空指针");
			return ;
		}
		// 从 parent->referencing 移除 child
		auto& refs = parent->referencing;
		std::erase(refs, child);

		// 从 child->referenced 移除 parent
		auto& parents = child->referenced;
		std::erase(parents, parent);
	}
};

template<typename T>
GCPtr<T> share_GCPtr(T* ptr, GCObject* owner)
{

	return GCPtr<T>(ptr,owner);
}




inline int GCSweep()
{
	for (auto obj : GCAllObjects)
	{
		obj->bMarked = false;
	}
	int cnt = 0;
	for (GCObject* obj : GCAllObjects)
	{
		if (!obj->bMarked)
		{
			delete obj;
			cnt++;
		}
	}
	return cnt;
}

inline void GCMark(GCObject* gc_object)
{
	//对象不存在 or 已被标记
	//if (!gc_object || gc_object->bMarked) return;
	gc_object->bMarked = true;
	for (auto child : gc_object->referencing)
	{
		GCMark(child);
	}
}

// template<class T>
// GCPtr<T> new_GCPtr(GCObject* Outer = nullptr) {
// 	return GCPtr<T>(Outer);
// }
// template< class InputMap >
// InputMap* new_GCPtr(
// 	GCObject* Outer = nullptr
// );


// template<typename T, typename ...Args>
// class GCPtr
// {
// 	T* ptr;
// 	GCObject* outer;
// 	// 构造
// public:
// 	GCPtr() : ptr(nullptr), outer(nullptr) {}
//
// 	explicit GCPtr(GCObject* outer, Args&&... args)
// 	{
// 		// if (!std::is_constructible<T, Args...>::value)
// 		// {
// 		// 	//abi::__cxa_demangle(typeid(T).name());
// 		// 	Log("参数列表无法匹配+"+ std::string(typeid(T).name()) + "  的构造函数");
// 		//
// 		// }
// 		static_assert(std::is_base_of_v<GCObject, T>, "T 必须继承自 GCObject");
// 		static_assert(std::is_constructible_v<T, Args...>,"参数列表无法匹配 T 的构造函数");
// 		ptr = new T(std::forward<Args>(args)...);
// 		this->outer = outer;
// 		if(outer)
// 		{
// 			outer->referencing.push_back(static_cast<GCObject*>(ptr));
// 		}
// 		else
// 		{
// 			Log("构造了野指针");
// 		}
// 		GCAllObjects.push_back(ptr);
// 	}
//
// 	// 禁止拷贝
// 	GCPtr(const GCPtr&) = delete;
// 	GCPtr& operator=(const GCPtr&)= delete;
//
// 	// 移动赋值
// 	GCPtr& operator=(GCPtr&& other) noexcept
// 	{
// 		if (this != &other)
// 		{
// 			delete ptr;
// 			ptr = other.ptr;
// 			outer = other.outer;
// 			other.ptr = nullptr;
// 			other.outer = nullptr;
// 		}
// 		return *this;
// 	}
// 	//移动构造
// 	GCPtr(GCPtr&& other) noexcept : ptr(other.ptr) ,outer(other.outer)
// 	{
// 		other.ptr = nullptr;
// 		other.outer = nullptr;
// 		//outer->references.push_back(ptr)
// 		// TODO: 移动时更新 outer->references，如果需要
// 	}
//
// 	~GCPtr() {}
//
// 	T* get() const { return ptr; }
// 	T* operator->() const
// 	{
// 		return ptr;
// 	}
// 	T operator*() const
// 	{
// 		return *ptr;
// 	}
// 	explicit operator bool() const noexcept
// 	{
// 		return ptr != nullptr;
// 	}
// };
