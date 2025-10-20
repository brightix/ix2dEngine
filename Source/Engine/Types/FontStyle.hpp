#pragma once

struct FontStyle
{
	std::shared_ptr<TTF_Font> font;
	int font_size;
	SDL_Color text_color;
	FontStyle()
	{
		font = nullptr;
		font_size = 24;
		text_color = {255,255,255,255};
	}
};

