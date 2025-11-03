#pragma once
#include <functional>

#include "FuncLib/ixStaticFuncLib.hpp"
#include "Classes/Object.hpp"
#include "FuncLib/GlobalVariable.hpp"

template<typename T>
class GCPtr
{
	T* ptr;
	Object* outer;
	// 构造
public:
	size_t id;
	GCPtr() : ptr(nullptr), outer(nullptr), id(0) { }
	//新对象
	explicit GCPtr(T* p, Object* outer) : ptr(p), outer(outer)
	{
		static_assert(std::is_base_of_v<Object, T>, "T 必须继承自 Object");
		if(!outer)
		{
			Log("构造了野指针");
		}
		GCLink(ptr,outer);
		GCAllObjects.push_back(ptr);

		if (GCAllObjects.size() == 25)
		{
			std::cout << std::endl;
		}
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
	[[nodiscard]] Object* GetOuter() const { return outer; }
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
	void SetOuter(Object* owner)
	{
		GCUnLink(ptr,outer);
		outer = owner;
		GCLink(ptr,outer);
	}
//绑定GC关系


	void Reset()
	{
		GCUnLink(ptr,outer);
		outer = nullptr;
		ptr = nullptr;
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
GCPtr<T> share_GCPtr(T* ptr, Object* owner)
{

	return GCPtr<T>(ptr,owner);
}

inline void AddToObject(Object* ptr)
{
	GCAllObjects.push_back(ptr);
}

void GCLink(Object* child, Object* parent);
void GCUnLink(Object* child, Object* parent);

// template<typename T>
// struct std::hash<GCPtr<T>> {
// 	size_t operator()(const GCPtr<T>& Ptr) const noexcept {
// 		return std::hash<T*>()(Ptr.Get()); // 使用裸指针地址做哈希
// 	}
// };