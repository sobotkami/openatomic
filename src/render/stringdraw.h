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
 * File:   stringdraw.h
 * Author: minji
 *
 * Created on 6. Januar 2013, 18:54
 */

#ifndef STRINGDRAW_H
#define	STRINGDRAW_H

#include <string>
#include <render/objectdraw.h>
#include <def/colors.h>
#include <singletons/fontsingleton.h>
using namespace std;

class CStringDraw : public CObjectDraw {
public:

    void drawString(Sint16 x, Sint16 y, string str);
    void drawString(Sint16 x, Sint16 y, const char * str);
    void drawString(Sint16 x, Sint16 y, string str, imagecolors color);
    void drawString(Sint16 x, Sint16 y, const char * str, imagecolors color);

    Uint16 getWidth(string str);
    Uint16 getWidth(const char * str);

    static CStringDraw *instance() {
        if (!s_instance)
            s_instance = new CStringDraw;
        return s_instance;
    }

private:
    static CStringDraw *s_instance;
    CFonReader * font;

    CStringDraw() {
        er.setLevel(LOG_INFO);
        er.report(LOG_INFO, _("%s: CStringDraw()\n"), AT);

        if (SDL_GetVideoSurface() == NULL) {
            er.report(LOG_INFO, _("%s: Called SDL_Init()?\n"), AT);
            throw;
        }
    }

};

#endif	/* STRINGDRAW_H */

