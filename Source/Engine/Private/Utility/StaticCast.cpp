#include "Utilities/FuncLib/StaticCast.hpp"
#include "Classes/Actor.hpp"

GCObject * AsGCObject(Actor *a)
{
    return a;
}

Actor *AsActor(GCObject* a)
{
    return static_cast<Actor*>(a);
}
