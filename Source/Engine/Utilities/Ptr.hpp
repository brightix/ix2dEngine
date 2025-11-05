#pragma once
#include <vector>
#include "Classes/Object.hpp"

// template<typename T>
// class PtrBase
// {
// public:
//     T* ptr;
// };
// inline std::vector<PtrBase<Object>*> GlobalPtr;
//



//
// template<typename T>
// class Ptr : public PtrBase<Object>
// {
// public:
//     Ptr()
//     {
//         GlobalPtr.push_back(this);
//     }
//
//     ~Ptr()
//     {
//         std::erase(GlobalPtr,this);
//     }
//
//     Ptr& operator=(T* other_ptr)
//     {
//         //static_assert(std::is_polymorphic_v<T> && std::is_polymorphic_v<U>, "dynamic_cast requires polymorphic types");
//
//         ptr = other_ptr;
//         return *this;
//     }
//
//     template<typename U>
//     Ptr& operator=(U* other_ptr)
//     {
//         static_assert(std::is_polymorphic_v<T> && std::is_polymorphic_v<U>, "dynamic_cast requires polymorphic types");
//
//         ptr = dynamic_cast<T*>(other_ptr);
//         return *this;
//     }
//
//     bool operator==(const Ptr<T>& other) const noexcept
//     {
//         return ptr == other.ptr;
//     }
//
//     bool operator!=(const Ptr<T>& other) const noexcept
//     {
//         return ptr != other.ptr;
//     }
//
//     // 还可以加上隐式转换（可选）
//     explicit operator bool() const noexcept { return ptr != nullptr; }
// };
