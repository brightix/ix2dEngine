#pragma once
#include "Classes/Object.hpp"
#include "Types/FRect.hpp"
#include "Utilities/GCPtr.hpp"
#include "Utilities/FuncLib/SystemLib.hpp"

#ifdef DebugMod
    #define DebugName(NAME) Name(NAME)
#else
    #define DebugName(NAME)
#endif

#define AUTO_COMPONENT(name) \
std::unordered_map<std::string, Widget*> name##_cache; \
GCPtr<ComponentManager> name


inline std::unordered_map<size_t,size_t> NameTable;
inline std::unordered_map<size_t,GCPtr<Object>> allObject;

template<typename T>
class GCWeakPtr;

struct Name
{
    Name(const std::string& name) : name_serial(0)
    {
        name_hash = ix::Hash(name.c_str());
        auto it = NameTable.find(name_hash);

        if (it == NameTable.end())
        {
            name_serial = 0;
        }
        else
        {
            name_serial = it->second;
            it->second++;
        }
    }
    explicit operator bool() const
    {
        return name_serial != -1;
    }
    bool operator==(const Name& other) const
    {
        return name_hash == other.name_hash && name_serial == other.name_serial;
    }
    size_t name_hash;
    size_t name_serial;
};

class Widget;

struct PanelSlot : public Object
{
    GCPtr<Widget> widget;
    FRect display_area;

    PanelSlot();
};
