#include <utility>

#include "Classes/Object.hpp"

#include "Utilities/GCPtr.hpp"
using namespace std;
Object::Object()
{
	id = glo_id++;
	Global_Object_Registry[id] = this;
}

void Object::AddCustomEvent(const std::string& event_name, EventMethod&& event_method)
{
    event_system.AddEvent(Event(event_name,event_method));
}

void Object::CallEvent(const std::string& event_name, std::optional<EventParams> params)
{
    event_system.CallEvent(event_name, std::move(params));
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
    dispatcher_system.BindEventTo(target, event_name, event);
}

void Object::ListenDispatcher(Object* target, const std::string &event_name, Event event)
{
	if (event.event_name.empty())
	{
		event.event_name = event_name;
	}
	target->dispatcher_system.BindEventTo(target, event_name, event);
}

Object::~Object()
{
	Global_Object_Registry.erase(id);
}

bool Object::IsActive() const
{
	return Global_Object_Registry.contains(id) && !Global_Object_Registry[id]->is_pending_kill;
}

std::string Object::GetClassName() const
{
	return class_name;
}

void Object::SetName(const std::string& new_name)
{
	name = new_name;
}




