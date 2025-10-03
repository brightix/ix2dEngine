#pragma once
#include <utility>

template<typename T, typename U>
inline std::pair<bool,T*> Cast(U* Before)
{
    if (T* t = dynamic_cast<T*>(Before))
    {
        return std::make_pair(true,t);
    }
    return std::make_pair(false,nullptr);
}
