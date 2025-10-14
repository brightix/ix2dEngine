#include "Classes/SubSystem/Sub/GarbageCollection.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/GCObject.hpp"
#include "Utilities/GCPtr.hpp"

GarbageCollection::GarbageCollection()
{
    
}

void GarbageCollection::GCMark(GCObject *gc_object)
{
	//对象不存在 or 已被标记
	if (!gc_object || gc_object->bMarked || gc_object->is_pending_kill) return;
	gc_object->bMarked = true;
	for (auto child : gc_object->referencing)
	{
		GCMark(child);
	}
}

int GarbageCollection::GCSweep()
{
	for (auto& obj : GCAllObjects)
	{
		obj->bMarked = false;
	}
	GCMark(GetWorld());
	std::vector<GCObject *> temp;
	int size = GCAllObjects.size();
	//temp.reserve(size);
	int cnt = 0;

	std::vector<GCObject*> to_delete;
	to_delete.reserve(GCAllObjects.size());

	for (auto& obj : GCAllObjects)
	{
		if (!obj->bMarked)
		{
			to_delete.emplace_back(obj);
			cnt++;
		}
		else temp.emplace_back(obj);
	}
	for (int i = 0; i < to_delete.size(); i++)
	{
		delete to_delete[i];
	}

	GCAllObjects.swap(temp);
	return cnt;
}