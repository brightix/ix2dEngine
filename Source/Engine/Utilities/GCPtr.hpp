#pragma once
#include "../Classes/Core/GCObject.hpp"
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
		//AddToObject();
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
		}
		return *this;
	}


	//派生->基类  同样会造成GC回收
	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(nullptr) {}

	//给outer添加新对象引用
	GCPtr(const GCPtr& other, GCObject* outer)
	{
		ptr = other.ptr;
		this->outer = outer;
		GCLink(ptr,outer);
	}

	/// 以下弱引用 会导致被GC回收,所以不要使用这个方法初始化值，要用类自带的Spawn 或 Construct
	GCPtr(const GCPtr& other) : ptr(other.Get()), outer(nullptr) {}
	GCPtr& operator=(const GCPtr& other)
	{
		GCUnLink(ptr,outer);
		ptr = other.ptr;
		GClink(ptr,outer);
		return *this;
	}
	//
	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr& operator=(const GCPtr<U>& other) {
		GCUnLink(ptr,outer);
		ptr = other.Get();
		GCLink(ptr,outer);
		return *this;
	}
	//移动构造 [废弃] GC不安全
	GCPtr(GCPtr&& other) noexcept
	{
		ptr = other.ptr;
		outer = other.outer;
	} // noexcept : ptr(other.ptr) ,outer(nullptr) {}


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
	void SetOwner(GCObject* owner)
	{
		outer = owner;
		GCLink(ptr,owner);
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
	static void GCUnLink(GCObject* child, GCObject* parent)
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
