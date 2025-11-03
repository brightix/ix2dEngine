#pragma once
#include <unordered_map>
#include <vector>


class Object;
inline std::vector<Object*> GCAllObjects;
inline std::unordered_map<size_t,Object*> Global_Object_Registry;
