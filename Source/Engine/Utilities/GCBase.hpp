#pragma once
#include <vector>

#include "Types/Array.hpp"

struct GCObject;

class GCBase
{
public:
    virtual ~GCBase() {}
    virtual GCObject* GetPtr() { return nullptr; }
    virtual bool IsValid() { return false; }

    virtual void Reset()= 0;
};
inline Array<GCBase*> GlobalPtr;