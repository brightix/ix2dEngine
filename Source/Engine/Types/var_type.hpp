#pragma once
#include <string>

struct var_type
{
    var_type(){}
    virtual ~var_type(){}
    virtual std::string str()= 0;
};
