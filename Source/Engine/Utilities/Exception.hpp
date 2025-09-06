#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{
    std::string msg;
public:
    Exception(const std::string& msg) : msg(msg) {}
    const char* what() const noexcept override
    {
        return msg.c_str();
    }
};