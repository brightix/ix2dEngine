#include "Classes/Core/EventDispatcherSystem.hpp"

#include "Utilities/FuncLib/ixStaticFuncLib.hpp"
#include "Classes/Object.hpp"
using namespace std;
EventDispatcherSystem::EventDispatcherSystem(GCObject *outer_)
{
	outer = outer_;
}

const Dispatcher* EventDispatcherSystem::GetDispatcherType(const std::string& dispatcher_name) const
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Log("没有该事件");
		return nullptr;
	}
	return &it->first;
}

void EventDispatcherSystem::AddDispatcher(const std::string& event_name, const std::vector<std::type_index>& dispatcher_type)
{
#if DEBUG
	if (bound_dispatcher.contains(event_name))
	{
		Log("添加["+ event_name +"]事件错误：已有同名分发器写入事件系统");
	}
#endif

	//非Debug模式就不管了
	bound_dispatcher[event_name];
#if DEBUG
	if (delegate.contains(event_name))
	{
		Log("添加["+ event_name +"]事件错误：已有同名分发器写入事件系统");
	}
#endif

	//非Debug模式就不管了
	delegate[Dispatcher(event_name, dispatcher_type)];
}

void EventDispatcherSystem::AcceptDelegate(Object *target, std::string dispatcher_name, std::string event_name)
{
	//1. 找 调用者 是否存在这个分发器
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		LogWithLevel(Warning,"绑定到空事件上:  " + dispatcher_name);
		return ;
	}
	const auto delegate_event = target->event_system.GetEventByName(event_name);
	//2 找 委托方 是否有这个事件
	if (!delegate_event)
	{
		LogWithLevel(Warning,target->name + "没有注册过这个事件：" + dispatcher_name);
		return ;
	}

	if (!it->first.CheckType(delegate_event->GetType()))
	{
		Log(dispatcher_name + " 不匹配 " + event_name + "事件绑定已取消");
	}
	// 2.找到这个对象绑定在这个分发器的所有函数
	auto& events = it->second[target];
// #if DEBUG == 1
	auto event_it = events.Find(event_name);
	if (event_it != events.end())
	{
		LogWithLevel_f(Warning, "重复绑定事件,{} 试图将 {} 重复绑定到 {} 的 {} 上",target->name, event_name, outer->name, dispatcher_name);
		return;
	}
// #endif
	//不可以重复添加相同事件
	events.insert(event_name);
}

void EventDispatcherSystem::RemoveDelegate(Object* target, const std::string& dispatcher_name, const std::string& event_name)
{
	const auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Log_f("没有找到在 {} 中存在 {} 分发器",outer->name, dispatcher_name);
		return ;
	}
	auto target_it = it->second.find(target);
	if (target_it == it->second.end())
	{
		Log_f("没有找到在 {} 中有任何属于 {} 的事件", outer->name, target->name);
		return ;
	}
	auto& target_events = target_it->second;
	if (!target_events.remove(event_name))
	{
		Log_f("没有在 {} 中找到 {} 的 {} 的事件",outer->name, target->name, event_name);
	}
}

void EventDispatcherSystem::CallDispatcher(const std::string &dispatcher_name, TEventParams event_params)
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Log_f("没有找到事件分发器 {}",dispatcher_name);
		return ;
	}
	auto& funcs = it->second;
	for (auto& [p,events] : funcs)
	{
		GCWeakPtr<Object> obj = p;
		for (auto& event_name : events)
		{
			obj->CallEvent(event_name, event_params);
		}
	}
}

void EventDispatcherSystem::RemoveAllEventByDispatcher(Object* sender, const std::string &dispatcher_name)
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Log_f("没有找到事件分发器 {}",dispatcher_name);
		return ;
	}
	it->second.erase(sender);
}
void EventDispatcherSystem::RemoveAllEventByEventName(Object* sender, const std::string &dispatcher_name,const std::string& event_name)
{
	auto it = delegate.find(dispatcher_name);
	if (it == delegate.end())
	{
		Log_f("没有找到事件分发器 {}",dispatcher_name);
		return ;
	}
	auto sender_it = it->second.find(sender);
	if (sender_it == it->second.end())
	{
		Log_f("没有找到任何 {} 绑定到 {} 的 {} 事件",sender->name, dispatcher_name, event_name);
	}
	auto& events = sender_it->second;
	for (auto& event : events)
	{
		if (event == event_name)
		{
			events.remove(event);
		}
	}
}


//
// // delegate_set
// void EventDispatcherSystem::DelegateEvent(Object *obj, std::string dispatcher_name, std::string event_name)
// {
// 	auto it = delegate.find(dispatcher_name);
// 	if (it == delegate.end())
// 	{
// 		LogWithLevel(Warning,"绑定到空事件上:  " + dispatcher_name);
// 		return ;
// 	}
// #if DEBUG == 1
// 	auto event_it = it->second[obj].find(event_name);
// 	if (event_it != it->second[obj].end())
// 	{
// 		Log_f("重复绑定事件,{} 试图将 {} 绑定到 {} 的 {} 上",obj->name,event_name,outer->name,dispatcher_name);
// 	}
// #else
// 	it->second[obj].insert(event_name);
// #endif
// }
//
// void EventDispatcherSystem::IgnoreDispatcher(Object* target, const std::string& dispatcher_name, const std::string& event_name)
// {
// 	const auto it = delegate.find(dispatcher_name);
// 	if (it == delegate.end())
// 	{
// 		Log_f("没有找到在 {} 中存在 {} 分发器",outer->name, dispatcher_name);
// 		return ;
// 	}
// 	auto target_it = it->second.find(target);
// 	if (target_it == it->second.end())
// 	{
// 		Log_f("没有找到在 {} 中有任何属于 {} 的事件", outer->name, target->name);
// 		return ;
// 	}
// 	auto event_it = target_it->second.find(event_name);
// 	if (event_it == target_it->second.end())
// 	{
// 		Log_f("没有找到在 {} 的事件", target->name);
//	    return ;
// 	}
// 	target_it->second.erase(event_it);
// }
// void EventDispatcherSystem::CallDispatcher(const std::string& event_name, std::optional<EventParams> event_params)
// {
// 	auto it = bound_dispatcher.find(event_name);
//
// 	if (it != bound_dispatcher.end() && !it->second.empty())			//找到事件名 并且 具有绑定的事件
// 	{
// 		for (auto& [owner,Events] : it->second)
// 		{
//
// 			if (owner.Peek())					//验证对象是否有效
// 			{
// 				for (auto& e : Events)
// 				{
// 					e(std::move(event_params));
// 				}
// 			}
// 			else
// 			{
// 				it->second.erase(owner);
// 			}
// 			//Log("执行了  "+ event_name + "  的事件！");
// 		}
// 	}
// }
//在这个事件系统里绑定其他对象的事件
// void EventDispatcherSystem::DelegateEvent(Object *obj, const std::string& bounded_name, Event event)
// {
//     auto it = bound_dispatcher.find(bounded_name);
//     if (it != bound_dispatcher.end())
//     {
//     	//如果名字为空，就设置为 "nameless" + 绑定事件名
//     	if (event.event_name.empty())
//     	{
// 			event.event_name = "nameless_" + bounded_name;
//     	}
//         bound_dispatcher[bounded_name][obj].emplace_back(event);
//     }
//     else
//     {
//         Log("Bind to Not Created Event \"" + bounded_name + "\"");
//     }
// }