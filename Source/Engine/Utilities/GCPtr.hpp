#pragma once
#include "../Classes/Core/GCObject.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include <cxxabi.h>
inline std::vector<GCObject*> GCAllObjects;
inline std::unordered_map<size_t,GCObject*> Global_GCObject_Registry;
#include <functional>

template<typename T>
struct std::hash<GCPtr<T>> {
	size_t operator()(const GCPtr<T>& Ptr) const noexcept {
		return std::hash<T*>()(Ptr.Get()); // 使用裸指针地址做哈希
	}
};


template<typename T>
class GCPtr
{
	T* ptr;
	GCObject* outer;
	// 构造
public:
	size_t id;
	GCPtr() : ptr(nullptr), outer(nullptr), id(0) { }
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
		//AddToObject();
		id = p->id;
	}
	// 移动赋值 [强引用] 给make_GCPtr用的 专门给构造赋值用
	GCPtr& operator=(GCPtr&& other) noexcept
	{
		if (this != &other)
		{
			GCUnLink(other.ptr,other.outer);
			ptr = other.ptr;
			outer = other.outer;
			GCLink(ptr,outer);
			id = other->id;
		}
		return *this;
	}

	template<typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { } // 支持向上转换

	//派生->基类  gc链不变，有释放风险
	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr(GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { }

	/// 以下弱引用 会导致被GC回收,所以不要使用这个方法初始化值，要用类自带的Spawn 或 Construct
	GCPtr(GCPtr& other) : ptr(other.Get()), outer(other.outer), id(other.id) { }

	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr& operator=(GCPtr<U>& other)
	{
		ptr = other.Get();
		outer = other.GetOuter();
		id = other.id;
		return *this;
	}

	GCPtr(const GCPtr& other) : ptr(other.Get()), outer(other.outer), id(other.id) { }

	template<typename U>
	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { }

	//assets
	T* Get() const { return ptr; }
	[[nodiscard]] GCObject* GetOuter() const { return outer; }
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
	void SetOuter(GCObject* owner)
	{
		GCUnLink(ptr,outer);
		outer = owner;
		GCLink(ptr,outer);
	}
//绑定GC关系
	static void GCLink(GCObject* child, GCObject* parent)
	{
		if (!child || !parent)
		{
			//Log("GCLink 绑定到空指针");
			//std::cout << "绑定到空指针" << std::endl;
			return ;
		}
		child->referenced.push_back(parent);
		parent->referencing.push_back(child);
	}
	void GCUnLink(GCObject* child, GCObject* parent)
	{
		if (!child || !parent)
		{
			//Log("GCLink 绑定到空指针");
			//std::cout << "接触到空指针" << std::endl;
			return ;
		}
		// 从 parent->referencing 移除 child
		auto& refs = parent->referencing;
		std::erase(refs, child);

		// 从 child->referenced 移除 parent
		auto& parents = child->referenced;
		std::erase(parents, parent);
	}
	bool IsValid()
	{
		return ptr != nullptr;
	}
	bool operator==(const GCPtr& other) const
	{
		return ptr == other.ptr;
	}
};



template<typename T>
GCPtr<T> share_GCPtr(T* ptr, GCObject* owner)
{

	return GCPtr<T>(ptr,owner);
}

inline void AddToObject(GCObject* ptr)
{
	GCAllObjects.push_back(ptr);
}