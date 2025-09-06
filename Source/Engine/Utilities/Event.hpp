#pragma once
#include <functional>

#include "Types/EventParam.hpp"


struct Event
{
    std::string event_name;
    std::function<void(std::optional<EventParams>)> event_func;

    explicit Event(const std::string& event_name, std::function<void(std::optional<EventParams>)> event_func);

    void Execute(std::optional<EventParams>&& event_params = std::nullopt);

    ~Event()= default;
};

