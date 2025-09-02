#pragma once
#include <string>

class Object
{
    std::string name;
    Object* Self_;
    //事件分发器

public:
    Object();
    Object* GetSelfPtr();
};