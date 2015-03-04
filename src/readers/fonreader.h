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


#ifndef FONREADER_H
#define FONREADER_H

#include "../def/sdl.h"
#include "filereader.h"
#include <debug/errorreporter.h>
#include "../def/colors.h"
#include "../global.h"
#include <string>
#include <vector>
using namespace std;

typedef struct {
    Uint32 width;
    Uint32 offset;
} FontInfo;

typedef struct {
    Uint32 num;
    Uint32 height;
    Uint32 spacing;
    vector<FontInfo> fi;
    char * data;
} FontData;

using namespace std;

class CFonReader
{
private:
    SDL_Surface * symbols[256][CCOLORS];
    CFileReader f;
    SDL_RWops * fp;
    Uint32 filesize;
    FontData font;

    CErrorReporter er;
public:
    SDL_Surface * getSymbol ( Uint8 s );
    SDL_Surface * getSymbol ( Uint8 s, imagecolors color );
    Sint16 getSpacing();
    Uint16 getSymbolWidth ( Uint8 s );
    Uint16 getStringWidth ( string str );
    Uint16 getSymbolHeight();
    Uint8 read ( string filename );
    CFonReader();
    ~CFonReader();
};

#endif // FONREADER_H
