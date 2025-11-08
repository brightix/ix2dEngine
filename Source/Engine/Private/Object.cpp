#include "Classes/Object.hpp"

using namespace std;
Object::Object() : dispatcher_system(this)
{
}

void Object::Construct()
{
    RegisterEvents();
	RegisterDispatchers();
}

// void Object::AddCustomEvent(const std::string& event_name, EventMethod&& event_method)
// {
//     event_system.AddEvent(Event(event_name,event_method));
// }

void Object::AddCustomEvent(Event&& event_method)
{
	event_system.AddEvent(std::forward<Event>(event_method));
}

void Object::AddCustomEvent(const std::string& event_name,Event&& event)
{
	event.event_name = event_name;
	event_system.AddEvent(std::forward<Event>(event));
}

void Object::CallEvent(const std::string& event_name, const TEventParams &params)
{
    event_system.CallEvent(event_name, params);
}

void Object::AddDispatcher(const std::string& event_name, const std::vector<std::type_index>& event_type)
{
    dispatcher_system.AddDispatcher(event_name, event_type);
}

void Object::AcceptDelegate(Object* delegate_obj, const std::string& dispatcher_name, const std::string& event_name)
{
	dispatcher_system.AcceptDelegate(delegate_obj, dispatcher_name,event_name);
}

void Object::RemoveDelegate(Object* dispatcher_owner, const std::string &dispatcher_name, const std::string& event_name)
{
    dispatcher_owner->dispatcher_system.RemoveDelegate(this, dispatcher_name, event_name);
}

void Object::CallDispatcher(const std::string& event_name, TEventParams event_params)
{
	dispatcher_system.CallDispatcher(event_name, std::move(event_params));
}

void Object::ListenDispatcher(Object* target, const std::string &dispatcher_name, const std::string& event_name)
{
	if (event_name.empty())
	{
		Log("绑定空事件");
		return ;
	}
	target->AcceptDelegate(this, dispatcher_name, event_name);
}

void Object::IgnoreDispatcher(Object* target, const std::string& dispatcher_name)
{
	if (dispatcher_name.empty())
	{
		Log("解绑到 空分发器 ");
		return ;
	}
	target->dispatcher_system.RemoveAllEventByDispatcher(this, dispatcher_name);
}

void Object::IgnoreDispatcherEvent(Object* target, const std::string& dispatcher_name, const std::string& event_name)
{
	if (dispatcher_name.empty())
	{
		Log("解绑到 空分发器 ");
		return ;
	}
	target->dispatcher_system.RemoveAllEventByEventName(this, dispatcher_name, event_name);
}

const std::string&& Object::ValidEvent(const std::string&& event_name) const
{
	auto it = event_system.events.find(event_name);
	if (it == event_system.events.end())
	{
		Log(name + " 没有事件 " + event_name);
	}
	return std::move(event_name);
}




