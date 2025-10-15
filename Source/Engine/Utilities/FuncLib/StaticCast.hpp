#pragma once
#include <utility>

template<typename T, typename U>
T* Cast(U* Before)
{
    if (T* t = dynamic_cast<T*>(Before))
    {
        return t;
    }
    return nullptr;
}
