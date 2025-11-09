#pragma once
#include "GCBase.hpp"
#include "Classes/Core/GCObject.hpp"
#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
inline std::vector<GCObject*> GCAllObjects;
inline std::unordered_map<size_t,GCObject*> Global_GCObject_Registry;
#include <functional>



template<typename T>
class GCPtr : public GCBase
{
	T* ptr = nullptr;
	// 构造
public:
	size_t id = 0;
	GCPtr() : GCPtr(nullptr) {}
	GCPtr(T* other_ptr)
	{
		if (other_ptr)
		{
			ptr = other_ptr;
			id = ptr->id;
		}
		GlobalPtr.insert(this);
	}
	~GCPtr()
	{
		GlobalPtr.remove(this);
	}
	//新对象

	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
	GCPtr& operator=(GCPtr<U>& other)
	{
		ptr = other.Get();
		id = other.id;
		return *this;
	}
	GCPtr& operator=(T* other) noexcept {
		ptr = other;
		id = other ? other->id : 0;
		return *this;
	}
	// ✅ 拷贝构造
	GCPtr(const GCPtr& other) : GCBase()
	{
		ptr = other.ptr;
		id = other.id;
		GlobalPtr.insert(this);
	}

	GCPtr& operator=(const GCPtr& other) {
		if (this != &other) {
			ptr = other.ptr;
			id = other.id;
		}
		return *this;
	}

	// ✅ 移动构造
	GCPtr(GCPtr&& other) noexcept {
		ptr = other.ptr;
		id = other.id;
		other.ptr = nullptr;
	}

	// ✅ 移动赋值
	GCPtr& operator=(GCPtr&& other) noexcept {
		if (this != &other) {
			ptr = other.ptr;
			id = other.id;
			other.ptr = nullptr;
		}
		return *this;
	}
	template<typename U, typename = std::enable_if_t<
		std::is_base_of_v<U, T> || std::is_base_of_v<T, U>
	>>
	GCPtr(const GCPtr<U>& other) : ptr(static_cast<T*>(other.Get())), id(other.id)
	{
		GlobalPtr.insert(this);
	}


	//assets
	T* Get() const { return ptr; }
	GCObject* GetPtr() override
	{
		//验证是否存活
		if (Global_GCObject_Registry.contains(id))
		{
			return reinterpret_cast<GCObject *>(ptr);
		}
		return nullptr;
	}

	template<typename U>
	GCPtr<U> Cast()
	{
		U* p = dynamic_cast<U*>(ptr);
		if (!p)
		{
			LogWithLevel(FatalError, "GC Cast失败");
			return nullptr;
		}
		return GCPtr<U>(p);
	}
	T* Peek() const
	{
		const auto it = Global_GCObject_Registry.find(id);
		if (it != Global_GCObject_Registry.end() && !it->second->is_pending_kill)
			return ptr;

		return nullptr;
	}
	void Reset() override
	{
		ptr = nullptr;
		id = 0;
	}
	[[nodiscard]] GCObject* GetOuter() const { return ptr->outer; }
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
	bool IsValid()
	{
		return ptr != nullptr;
	}
	bool operator==(const GCPtr& other) const
	{
		return ptr == other.ptr;
	}
	bool operator==(const T*& other) const noexcept
	{
		return ptr == other;
	}
};
template<typename T>
bool operator==(const GCPtr<T>& a, const T* b) noexcept {
	return a.Get() == b;
}

template<typename T>
bool operator==(const T* a, const GCPtr<T>& b) noexcept {
	return a == b.Get();
}
template<typename T>
struct std::hash<GCPtr<T>> {
	size_t operator()(const GCPtr<T>& Ptr) const noexcept {
		return std::hash<T*>()(Ptr.Get()); // 使用裸指针地址做哈希
	}
};

//出作用域自动删除
// class GCStrongPtr : public GCPtr<T>
template<typename T>
class GCStrongPtr
{
	T* ptr = nullptr;
public:
	size_t id = -1;
	GCStrongPtr()= default;
	GCStrongPtr& operator=(T* other)
	{
		ptr = other;
		id = other ? other->id : 0;
		return *this;
	}
	T* operator->() const
	{
		return ptr;
	}
	~GCStrongPtr()
	{
		delete ptr;
	}
};


// template<typename T>
// class GCPtr
// {
// 	T* ptr;
// 	GCObject* outer;
// 	// 构造
// public:
// 	size_t id;
// 	GCPtr() : ptr(nullptr), outer(nullptr), id(0)
// 	{
// 		GlobalPtr.push_back(this);
// 	}
// 	//新对象
// 	explicit GCPtr(T* p, GCObject* outer) : ptr(p), outer(outer)
// 	{
// 		static_assert(std::is_base_of_v<GCObject, T>, "T 必须继承自 GCObject");
// 		if(!outer)
// 		{
// 			Log("构造了野指针");
// 		}
// 		GCLink(ptr,outer);
// 		GCAllObjects.push_back(ptr);
//
// 		if (GCAllObjects.size() == 25)
// 		{
// 			std::cout << std::endl;
// 		}
// 		//AddToObject();
// 		id = p->id;
// 	}
// 	// 移动赋值 [强引用] 给make_GCPtr用的 专门给构造赋值用
// 	GCPtr& operator=(GCPtr&& other) noexcept
// 	{
// 		if (this != &other)
// 		{
// 			GCUnLink(other.ptr,other.outer);
// 			ptr = other.ptr;
// 			outer = other.outer;
// 			GCLink(ptr,outer);
// 			id = other->id;
// 		}
// 		return *this;
// 	}
//
// 	template<typename U, typename = std::enable_if_t<std::is_base_of_v<U, T>>>
// 	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { } // 支持向上转换
//
// 	//派生->基类  gc链不变，有释放风险
// 	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
// 	GCPtr(GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { }
//
// 	/// 以下弱引用 会导致被GC回收,所以不要使用这个方法初始化值，要用类自带的Spawn 或 Construct
// 	GCPtr(GCPtr& other) : ptr(other.Get()), outer(other.outer), id(other.id) { }
//
// 	template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
// 	GCPtr& operator=(GCPtr<U>& other)
// 	{
// 		ptr = other.Get();
// 		outer = other.GetOuter();
// 		id = other.id;
// 		return *this;
// 	}
//
// 	GCPtr(const GCPtr& other) : ptr(other.Get()), outer(other.outer), id(other.id) { }
//
// 	template<typename U>
// 	GCPtr(const GCPtr<U>& other) : ptr(other.Get()), outer(other.GetOuter()), id(other.id) { }
//
// 	//assets
// 	T* Get() const { return ptr; }
// 	[[nodiscard]] GCObject* GetOuter() const { return outer; }
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
// 	void SetOuter(GCObject* owner)
// 	{
// 		GCUnLink(ptr,outer);
// 		outer = owner;
// 		GCLink(ptr,outer);
// 	}
// //绑定GC关系
// 	static void GCLink(GCObject* child, GCObject* parent)
// 	{
// 		if (!child || !parent)
// 		{
// 			//Log("GCLink 绑定到空指针");
// 			//std::cout << "绑定到空指针" << std::endl;
// 			return ;
// 		}
// 		child->referenced.push_back(parent);
// 		parent->referencing.push_back(child);
// 	}
// 	void GCUnLink(GCObject* child, GCObject* parent)
// 	{
// 		if (!child || !parent)
// 		{
// 			//Log("GCLink 绑定到空指针");
// 			//std::cout << "接触到空指针" << std::endl;
// 			return ;
// 		}
// 		// 从 parent->referencing 移除 child
// 		auto& refs = parent->referencing;
// 		std::erase(refs, child);
//
// 		// 从 child->referenced 移除 parent
// 		auto& parents = child->referenced;
// 		std::erase(parents, parent);
// 	}
// 	void Reset()
// 	{
// 		GCUnLink(ptr,outer);
// 		outer = nullptr;
// 		ptr = nullptr;
// 	}
// 	bool IsValid()
// 	{
// 		return ptr != nullptr;
// 	}
// 	bool operator==(const GCPtr& other) const
// 	{
// 		return ptr == other.ptr;
// 	}
// };



template<typename T>
GCPtr<T> share_GCPtr(T* ptr, GCObject* owner)
{

	return GCPtr<T>(ptr,owner);
}

inline void AddToObject(GCObject* ptr)
{
	GCAllObjects.push_back(ptr);
}