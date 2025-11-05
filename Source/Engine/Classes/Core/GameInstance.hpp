#pragma once
#include <vector>

#include "Utilities/Ptr.hpp"

class GameInstance
{
public:

    std::vector<GCPtr<Object>> GlobalPtr;
    GameInstance(){}
public:
    static GameInstance& Instance()
    {
        static GameInstance instance;
        return instance;
    }
};
