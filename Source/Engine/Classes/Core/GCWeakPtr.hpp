#pragma once

template<typename T>
struct GCPtr
{
    T* ptr;
    GCObject gc_ptr;
    T& operator*() const
    {

        if (gc_ptr)
        {
            return *ptr;
        }
        return nullptr;
    }
};