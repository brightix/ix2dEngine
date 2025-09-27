#include "Classes/Object.hpp"

#include <iostream>
using namespace std;
Object::Object() : Self(this) {}

Object* Object::GetSelfPtr()
{
    return this;
}

void Object::AddCustomEvent(const std::string& event_name, EventMethod&& event_method)
{
    event_system.AddEvent(Event(event_name,event_method));
}

void Object::CallEvent(const std::string& event_name, std::optional<EventParams> params)
{
    event_system.CallEvent(event_name, params);
}

void Object::AddEventDispatcher(const std::string& event_name, EventMethod&& event_method)
{
    dispatcher_system.AddEventDispatcher(event_name);
}

void Object::BindEvent(std::string dispatcher_name, EventMethod method)
{
	//dispatcher_system.
}

