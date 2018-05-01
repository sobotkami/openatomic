/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

/*
	SDL_image:  An example image loading library for use with SDL
	Copyright (C) 1997-2009 Sam Lantinga

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Sam Lantinga
	slouken@libsdl.org
*/

/*
 * PCX file reader:
 * Supports:
 *  1..4 bits/pixel in multiplanar format (1 bit/plane/pixel)
 *  8 bits/pixel in single-planar format (8 bits/plane/pixel)
 *  24 bits/pixel in 3-plane format (8 bits/plane/pixel)
 *
 * (The <8bpp formats are expanded to 8bpp surfaces)
 *
 * Doesn't support:
 *  single-planar packed-pixel formats other than 8bpp
 *  4-plane 32bpp format with a fourth "intensity" plane
 */

#ifndef PCXREADER_H
#define PCXREADER_H

#include <def/sdl.h>
#include <debug/errorreporter.h>
#include <string>
using namespace std;

struct PCXheader
{
	Uint8 Manufacturer;
	Uint8 Version;
	Uint8 Encoding;
	Uint8 BitsPerPixel;
	Sint16 Xmin, Ymin, Xmax, Ymax;
	Sint16 HDpi, VDpi;
	Uint8 Colormap[48];
	Uint8 Reserved;
	Uint8 NPlanes;
	Sint16 BytesPerLine;
	Sint16 PaletteInfo;
	Sint16 HscreenSize;
	Sint16 VscreenSize;
	Uint8 Filler[54];
};

class CPcxReader
{
private:
	SDL_RWops * fp;
	SDL_Surface * LoadPCX_RW ( SDL_RWops *src );
	
	CErrorReporter er;
public:
	SDL_Surface * read ( string filename );
	CPcxReader();
	~CPcxReader();
};

#endif // PCXREADER_H
