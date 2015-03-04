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
 * File:   objectdraw.h
 * Author: minji
 *
 * Created on 6. Januar 2013, 18:53
 */

#ifndef OBJECTDRAW_H
#define	OBJECTDRAW_H

#include <def/sdl.h>
#include <debug/errorreporter.h>

class CObjectDraw {
public:

    SDL_Rect* drawObject(SDL_Surface * img, Sint16 x, Sint16 y, bool flip);
    void clearRect(SDL_Rect* rect);
    void clearRect(Sint16 x, Sint16 y, Sint16 w, Sint16 h);

    CObjectDraw() {
        er.setLevel(LOG_INFO);
        er.report(LOG_INFO, _("%s: CObjectDraw()\n"), AT);

        if (SDL_GetVideoSurface() == NULL) {
            er.report(LOG_INFO, _("%s: Called SDL_Init()?\n"), AT);
            throw;
        }
    };
protected:
    CErrorReporter er;

};

#endif	/* OBJECTDRAW_H */

