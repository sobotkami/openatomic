/**
 * This file is part of OpenAtomic.
 *
 * OpenAtomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenAtomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAtomic. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * File:   stringdraw.cpp
 * Author: minji
 *
 * Created on 6. Januar 2013, 18:54
 */

#include <sys/types.h>

#include <render/stringdraw.h>

CStringDraw *CStringDraw::s_instance;

void CStringDraw::drawString (Sint16 x, Sint16 y, string str)
{
	drawString(x, y, str, cwhite);
}

void CStringDraw::drawString (Sint16 x, Sint16 y, const char * str)
{
	drawString(x, y, str, cwhite);
}

void CStringDraw::drawString (Sint16 x, Sint16 y, string str, imagecolors color)
{
	drawString(x, y, str.c_str(), color);
}

void CStringDraw::drawString (Sint16 x, Sint16 y, const char* str, imagecolors color)
{
	Uint16 offset = 0;
	Sint16 spacing = 0;
	SDL_Surface * symbol = NULL;
	Uint8 s = 0;
	Sint16 size = 0;

	spacing = CFontSingleton::instance()->font.getSpacing() - 2; // 2 pix se umele pridavaji
	size = strlen(str);

	for (Sint16 i = 0; i < size; ++i)
	{
		s = str[i];
		symbol = CFontSingleton::instance()->font.getSymbol(s, color);
		if (symbol != NULL)
		{
			drawObject(symbol, x + offset, y, false); // on error resume next
			offset += CFontSingleton::instance()->font.getSymbolWidth(s);
			offset += spacing;
		}
	}
}

Uint16 CStringDraw::getWidth (string str)
{
	return getWidth(str.c_str());
}

Uint16 CStringDraw::getWidth (const char * str)
{
	Uint16 strWidth = 0;
	Sint16 spacing = CFontSingleton::instance()->font.getSpacing() - 2; // 2 pix se umele pridavaji;
	ssize_t size = strlen(str);

	for (ssize_t i = 0; i < size; ++i)
	{
		strWidth += CFontSingleton::instance()->font.getSymbolWidth(str[i]);
		strWidth += spacing;
	}

	return strWidth;
}
