#include "Asset/Asset.hpp"

void Asset::NativeSetOuter(GCObject *new_outer)
{
    GCObject::SetOuter(new_outer);

}
