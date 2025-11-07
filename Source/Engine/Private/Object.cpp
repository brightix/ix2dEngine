#include "Classes/Object.hpp"

#include <iostream>
using namespace std;
Object::Object()
{
}

void Object::Construct()
{
    RegisterEvents();
}

void Object::AddCustomEvent(const std::string& event_name, EventMethod&& event_method)
{
    event_system.AddEvent(Event(event_name,event_method));
}

void Object::CallEvent(const std::string& event_name, const std::optional<EventParams> &params)
{
    event_system.CallEvent(event_name, params);
}

void Object::AddEventDispatcher(const std::string& event_name, EventMethod&& event_method)
{
    dispatcher_system.AddEventDispatcher(event_name);
}

void Object::BindEventToDispatcher(Object* target, const std::string &event_name,Event event)
{
    if (event.event_name.empty())
    {
        event.event_name = event_name;
    }
    dispatcher_system.BindEventTo(target, event_name, std::move(event));

    //target->dispatcher_system.DelegateEvent(target, dispatcher_name);
}

void Object::IgnoreDispatcher(Object* target, const std::string &dispatcher_name)
{
    target->dispatcher_system.IgnoreDispatcher(target, dispatcher_name);
}

void Object::ListenDispatcher(Object* target, const std::string &event_name, Event event)
{
	if (event.event_name.empty())
	{
		event.event_name = event_name;
	}
	target->dispatcher_system.BindEventTo(target, event_name, std::move(event));
}




