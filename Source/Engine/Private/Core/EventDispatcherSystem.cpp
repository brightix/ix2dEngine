#include "Classes/Core/EventDispatcherSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/Object.hpp"
using namespace std;
EventDispatcherSystem::EventDispatcherSystem(GCObject *outer_)
{
	outer = outer_;
}

void EventDispatcherSystem::AddEventDispatcher(const std::string& event_name)
{

#if DEBUG
	if (bound_dispatcher.contains(event_name))
	{
		Log("添加["+ event_name +"]事件错误：已有相同事件写入事件系统");
	}
#endif

	//非Debug模式就不管了
	bound_dispatcher[event_name];
    // if (!bound_dispatcher.emplace(event_name,Event(event_name,event_method)).second)
    // {
    //
    // }
}

//在这个事件系统里绑定其他对象的事件
void EventDispatcherSystem::BindEventTo(Object *obj, const std::string& bounded_name, Event event)
{
    auto it = bound_dispatcher.find(bounded_name);
    if (it != bound_dispatcher.end())
    {
    	//如果名字为空，就设置为 "nameless" + 绑定事件名
    	if (event.event_name.empty())
    	{
			event.event_name = "nameless_" + bounded_name;
    	}
        bound_dispatcher[bounded_name][obj].emplace_back(event);
    }
    else
    {
        Log("Bind to Not Created Event \"" + bounded_name + "\"");
    }
}

std::optional<Event> EventDispatcherSystem::GetEvent(Object* obj, const std::string& event_name)
{
	// auto it_by_obj = GCPtr(obj);
	//
	// if (bound_dispatcher.contains(event_name) && bound_dispatcher[event_name].contains(it_by_obj))
	// {
	// 	return bound_dispatcher[event_name][it_by_obj];
	// }
	return std::nullopt;
}

void EventDispatcherSystem::CallDispatcher(const std::string& event_name, std::optional<EventParams> event_params)
{
    auto it = bound_dispatcher.find(event_name);

    if (it != bound_dispatcher.end() && !it->second.empty())			//找到事件名 并且 具有绑定的事件
    {
        for (auto& [owner,Events] : it->second)
        {

        	if (owner.Peek())					//验证对象是否有效
        	{
				for (auto& e : Events)
				{
					e(std::move(event_params));
				}
        	}
        	else
        	{
        		it->second.erase(owner);
        	}
            //Log("执行了  "+ event_name + "  的事件！");
        }
    }
    // else
    // {
    //     Log("没有名为  "+ event_name + "  的事件！");
    // }
}

void EventDispatcherSystem::DelegateEvent(Object *obj, std::string dispatcher_name, std::string event_name)
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		LogWithLevel(Warning,"绑定到空事件上:  " + dispatcher_name);
		return ;
	}
#if DEBUG == 1
	auto event_it = it->second[obj].find(event_name);
	if (event_it != it->second[obj].end())
	{
		Logf("重复绑定事件,{} 试图将 {} 绑定到 {} 的 {} 上",obj->name,event_name,outer->name,dispatcher_name);
	}
#else
	it->second[obj].insert(event_name);
#endif
}

void EventDispatcherSystem::CallDelegate(const std::string &dispatcher_name)
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Logf("没有找到事件分发器 {}",dispatcher_name);
		return ;
	}
	for (auto& funcs = it->second; auto& [p,events] : funcs)
	{
		GCWeakPtr<Object> obj = p;
		for (auto& event_name : events)
		{
			obj->CallEvent(event_name);
		}
	}
}

