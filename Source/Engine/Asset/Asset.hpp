#pragma once
#include "Classes/Core/GCObject.hpp"

class Asset : public GCObject
{
public:
    Asset(){}
    void NativeSetOuter(GCObject *new_outer) override;
};
