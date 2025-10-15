#include "Classes/SubSystem/GarbageCollection.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/GCObject.hpp"
#include "Utilities/GCPtr.hpp"

GarbageCollection::GarbageCollection()
{
    name = "GarbageCollection";
}

void GarbageCollection::GCMark(GCObject *gc_object)
{
	//对象不存在 or 已被标记
	if (!gc_object || gc_object->bMarked) return;
	if (gc_object->is_pending_kill)
	{
		for (auto parent : gc_object->referenced)
		{
			std::erase(parent->referencing,this);
		}
		return;
	}
	gc_object->bMarked = true;
	for (auto child : gc_object->referencing)
	{
		GCMark(child);
	}
}

int GarbageCollection::GCSweep()
{
	//清理标记
	for (auto& obj : GCAllObjects)
	{
		if (obj)
		{
			obj->bMarked = false;
		}
	}
	GCMark(GetEngine());
	int cnt = 0;

	for (auto& obj : GCAllObjects)
	{
		//不可达路径删除
		if (obj && !obj->bMarked)
		{
			//to_delete.emplace_back(obj);
			std::cout << "移除了 " + obj->name << std::endl;
			delete obj;
			obj = nullptr;
			cnt++;
		}
	}
	Block += cnt;
	return cnt;
}