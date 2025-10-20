#pragma once
#include <string>

#include "Types/FontStyle.hpp"


struct TextInfo
{
	std::string context;
	FontStyle style;
    TextInfo();
	void SetText(const std::string& text)
	{
		context = text;
	}
	void SetTextStyle(const FontStyle& s)
	{
		style = s;
	}
	// void SetTextColor(const SDL_Color& new_color)
	// {
	// 	text_color = new_color;
	// }
    ~TextInfo()= default;
};

