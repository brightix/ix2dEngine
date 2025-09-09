#pragma once
#include "Window/GlobalMacros.hpp"

class Widget : public Object
{
std::vector<std::shared_ptr<Widget>> children;
public:
    Widget();
    ~Widget()= default;
	void AddChild(GCPtr<Widget> Target, GCPtr<Widget> UI);
};

