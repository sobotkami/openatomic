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

#ifndef FILEREADER_H
#define FILEREADER_H

#include <def/sdl.h>
#include <debug/errorreporter.h>

using namespace std;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define SWAP16(X)	(X)
#define SWAP32(X)	(X)
#else
#define SWAP16(X)	SDL_Swap16(X)
#define SWAP32(X)	SDL_Swap32(X)
#endif

class CFileReader
{
private:
	SDL_RWops *fp;

	CErrorReporter er;
public:
	CFileReader();
	CFileReader ( SDL_RWops * fp );

	Uint32 getMaxline();
	void   setMaxline ( Uint32 maxline );
	Uint32 getPos();
	Uint32 setRelPos ( Sint32 offset );

	Sint32 read32();
	Sint16 read16();
	Sint8  read8();
	char  *readBytes ( Uint32 count );
	bool   testByte ( Uint8 byte );

};

#endif // FILEREADER_H
