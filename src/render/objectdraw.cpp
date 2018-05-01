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
 * File:   objectdraw.cpp
 * Author: minji
 *
 * Created on 6. Januar 2013, 18:53
 */

#include <render/objectdraw.h>
#include <cassert>

SDL_Rect* CObjectDraw::drawObject (SDL_Surface* img, Sint16 x, Sint16 y, bool flip)
{
	//assert(img != NULL);
	if (img == NULL)
	{
		er.report(LOG_ERROR, "%s: drawObject(NULL, %d, %d, %d)\n", AT, x, y, flip);
		return NULL;
	}

	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = img->w;
	r.h = img->h;

	SDL_BlitSurface(img, NULL, SDL_GetVideoSurface(), &r);
	//	 SDL_UpdateRect ( screen, 0, 0, screen->w, screen->h );


	if (flip)
	{
		if ((SDL_GetVideoSurface()->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF)
		{
			SDL_Flip(SDL_GetVideoSurface());
		}
		else
		{
			SDL_UpdateRect(SDL_GetVideoSurface(), r.x, r.y, r.w, r.h);
		}
	}

	SDL_Rect* rect = &r;
	return rect;
}

void CObjectDraw::clearRect (SDL_Rect* rect)
{
	SDL_FillRect(SDL_GetVideoSurface(), rect, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));
}

void CObjectDraw::clearRect (Sint16 x, Sint16 y, Sint16 w, Sint16 h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	clearRect(&r);
}
