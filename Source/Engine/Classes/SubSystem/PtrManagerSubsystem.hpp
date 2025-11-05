#pragma once
#include <vector>

#include "Sub/GameInstanceSubsystem.hpp"

class Object;

class PtrManagerSubsystem : public GameInstanceSubsystem
{
    std::vector<Ptr<Object>> PtrObjects;
public:
    PtrManagerSubsystem()
    {
    }

    void AddObject(const Ptr<Object> &ptr)
    {
        PtrObjects.push_back(ptr);
    }
    void RemoveObject(const Ptr<Object> &ptr)
    {
        PtrObjects.erase(std::ranges::find(PtrObjects, ptr));
    }

};
