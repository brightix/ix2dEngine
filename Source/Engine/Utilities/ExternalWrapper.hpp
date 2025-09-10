#pragma once
#include "System/GCObject.hpp"

template<typename T,typename...Args>
struct ExternalWrapper : GCObject
{
	T* val;
public:
    ExternalWrapper(T* v) : val(v) {}
    ~ExternalWrapper()= default;
};

