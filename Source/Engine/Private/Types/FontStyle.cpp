#include "Types/FontStyle.hpp"
#include "System/Font.hpp"

FontStyle::FontStyle()
{
	font_size = 24;
	font = FontRenderer::Instance().GetFont();
	text_color = {255,255,255,255};
}

