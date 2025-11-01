#include "Classes/SubSystem/GarbageCollection.hpp"

#include "Classes/Core/GameEngine.hpp"
#include "Classes/Core/GameWorld.hpp"
#include "Classes/Core/GCObject.hpp"
#include "Utilities/GCPtr.hpp"
#include "Utilities/TracingUtility.hpp"

GarbageCollection::GarbageCollection()
{
    CNAME;
}

void GarbageCollection::GCMark(GCObject *gc_object)
{
	//对象不存在 or 已被标记
	if (!gc_object || gc_object->bMarked || gc_object->is_pending_kill) return;
	gc_object->bMarked = true;
	auto& children = gc_object->referencing;
	for (int i = 0; i<children.size(); i++)
	{
		//std::cout << children[i]->name << std::endl;
		GCMark(children[i]);
	}
	// for (auto& child : children)
	// {
	// 	std::cout << child->name << std::endl;
	// 	GCMark(child);
	// }
}

int GarbageCollection::GCSweep()
{
	TStart;
	//清理标记
	for (auto& obj : GCAllObjects)
	{
		if (obj)
		{
			obj->bMarked = false;
		}
	}
	GCMark(GetEngine()->GetGCRoot());
	int cnt = 0;

	for (auto& obj : GCAllObjects)
	{
		//不可达路径删除
		if (obj && !obj->bMarked)
		{
			//to_delete.emplace_back(obj);

			if (obj->class_name == "UnknownClass")
			{
				std::cout << "有未命名类" << std::endl;
			}
			//std::cout << "移除了 " + obj->name << std::endl;
			delete obj;
			obj = nullptr;
			cnt++;
		}
	}
	Block += cnt;
	TEnd;
	return cnt;
}