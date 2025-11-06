#pragma once

struct GCObject;
class Actor;
GCObject* AsGCObject(Actor *a);

Actor *AsActor(GCObject* a);