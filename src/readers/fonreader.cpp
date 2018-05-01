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

#include "fonreader.h"

CFonReader::CFonReader()
{
	fp = NULL;
	er.setLevel( LOG_ERROR );
}


CFonReader::~CFonReader()
{
	if ( fp != NULL )
		SDL_RWclose ( fp );
}

Uint8 CFonReader::read(string filename)
{
	FontInfo fi;

	SDL_Surface *result[CCOLORS];
	Uint16 *pixels[CCOLORS];
	SDL_PixelFormat * pxlformat = NULL;
	Uint16 pixBlank, pixBorder;
	Uint16 pixC[CCOLORS];
	Sint16 ox, oy;

	er.report( LOG_INFO, _("%s: Opening a file '%s'.\n"), AT, filename.c_str() );
	if ( ( fp = SDL_RWFromFile ( filename.c_str(), "rb" ) ) == NULL )
	{
		er.report( LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, filename.c_str() );
		return 1;
	}
	else
	{
		SDL_RWseek ( fp, 0, SEEK_END );
		filesize = SDL_RWtell ( fp );
		SDL_RWseek ( fp, 0, SEEK_SET );

		f = CFileReader ( fp );
	}

	

	if ( SDL_GetVideoSurface() == NULL )
	{
		er.report( LOG_ERROR, _("%s: SDL_GetVideoSurface() fail. Before SDL_Init()?\n"), AT );
		return 2;
	}
	
	pxlformat = SDL_GetVideoSurface()->format;

	pixC[cgreen  ] = SDL_MapRGB( pxlformat, 0x00, 0xff, 0x00 );
	pixC[cyellow ] = SDL_MapRGB( pxlformat, 0xff, 0xff, 0x00 );
	pixC[ccyan   ] = SDL_MapRGB( pxlformat, 0x00, 0xff, 0xff );
	pixC[cred	] = SDL_MapRGB( pxlformat, 0xff, 0x00, 0x00 );
	pixC[cblue   ] = SDL_MapRGB( pxlformat, 0x00, 0x00, 0xff );
	pixC[corange ] = SDL_MapRGB( pxlformat, 0xff, 0x7f, 0x00 );
	pixC[cmagenta] = SDL_MapRGB( pxlformat, 0xff, 0x00, 0xff );
	pixC[cpurple ] = SDL_MapRGB( pxlformat, 0x7f, 0x00, 0xff );
	pixC[cwhite  ] = SDL_MapRGB( pxlformat, 0xff, 0xff, 0xff );
	pixC[cblack  ] = SDL_MapRGB( pxlformat, 0x00, 0x00, 0x00 );

	pixBlank  = SDL_MapRGB( pxlformat, 0x55, 0x55, 0x55 );
	pixBorder = SDL_MapRGB( pxlformat, 0xF0, 0xF0, 0x05 );

	font.num = f.read32();
	font.height = f.read32();
	font.spacing = f.read32();
	f.read32(); // FontInfo dummy, 32b
	f.read32(); // Data dummy, 32b


	er.report( LOG_INFO, _("%s: Number of slices: %d, font height: %d, font spacing: %d\n"),
			   AT, font.num, font.height, font.spacing );

	for ( Uint32 i = 0; i < font.num; ++i )
	{
		fi.width = f.read32();
		fi.offset = f.read32();
		font.fi.push_back(fi);
	}

	Uint32 size = font.fi[ font.num - 1 ].offset + (font.fi[ font.num - 1 ].width + 7) / 8 * font.height;

//	 cout << "last: " << (Uint32)(font.num - 1) << ", size: " << (Uint32)size << endl;
	if ( ( font.data = (char *)malloc( size ) ) == NULL )
	{
		er.report( LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, size );
		exit ( 1 );
	}
	font.data = f.readBytes( size );

	ox = 1;
	oy = 0;

	for ( Uint32 i = 0; i < font.num; i++ )
	{
		if (font.height * font.fi[i].width != 0)
		{
			Uint32 offset = font.fi[i].offset;
			Uint32 bytesPerLine = (font.fi[i].width + 7) / 8;
			Uint32 pixelsPerLine = 0;
			Sint32 j;
			Sint32 h;

			for ( Uint32 c = 0; c < CCOLORS; ++c )
			{
				result[c] = SDL_CreateRGBSurface ( SDL_HWSURFACE,
													font.fi[i].width+2, font.height,
													pxlformat->BitsPerPixel,
													pxlformat->Rmask,
													pxlformat->Gmask,
													pxlformat->Bmask,
													pxlformat->Amask
												 );

				if ( result[c] == NULL )
				{
					er.report( LOG_ERROR, _("%s: CreateRGBSurface(): %s.\n"), AT, SDL_GetError() );
					return 3;
				}

				/* ???????????????????????????????????????????????????????????????????
				if ( SDL_MUSTLOCK( result[c] ) )
					SDL_LockSurface ( result[c] );
				 */

				pixels[c] = ( Uint16 * ) result[c]->pixels;
			}

			pixelsPerLine = result[0]->pitch/sizeof ( Uint16 );

			for (h = 0; h < (signed)font.height; h++)
			{
				for (j = 0; j < (signed)font.fi[i].width; j++)
				{
					if ( j < (signed)font.fi[i].width )
					{
						if ( font.data[offset + h * bytesPerLine + (j / 8)] & (1 << (7 - (j % 8))) )
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][(h + oy) * pixelsPerLine + (j + ox)] = pixC[c];
							}
						}
						else
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][(h + oy) * pixelsPerLine + (j + ox)] = pixBlank;
							}
						}
					}
					else
					{
						for ( Uint16 c = 0; c < CCOLORS; ++c )
						{
							pixels[c][(h + oy) * pixelsPerLine + (j + ox)] = pixBlank;
						}
					}
				}
			}

			for (h = 0; h < (signed)font.height; h++)
			{
				for ( Uint16 c = 0; c < CCOLORS; ++c )
				{
					pixels[c][(h + oy) * pixelsPerLine + 0] = pixBlank;
				}

				for ( Uint16 c = 0; c < CCOLORS; ++c )
				{
					pixels[c][(h + oy) * pixelsPerLine + result[0]->w-1] = pixBlank;
				}
			}

			for (h = 0; h < result[0]->h; h++)
			{
				for (j = 0; j < result[0]->w; j++)
				{
					/* ohraniceni zleva */
					if ( j >= 0 && j+1 < result[0]->w )
					{
						if ( pixels[0][h * pixelsPerLine + j] == pixBlank &&
							 pixels[0][h * pixelsPerLine + (j+1)] != pixBlank &&
							 pixels[0][h * pixelsPerLine + (j+1)] != pixBorder
						   )
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][h * pixelsPerLine + j] = pixBorder;
							}
						}
					}

					/* ohraniceni zprava */
					if ( j > 0 && j <= result[0]->w )
					{
						if ( pixels[0][h * pixelsPerLine + (j-1)] != pixBlank &&
							 pixels[0][h * pixelsPerLine + (j-1)] != pixBorder &&
							 pixels[0][h * pixelsPerLine + j] == pixBlank
						   )
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][h * pixelsPerLine + j] = pixBorder;
							}
						}
					}

					/* ohraniceni zhora */
					if ( h >= 0 && h+1 < result[0]->h )
					{
						if ( pixels[0][h * pixelsPerLine + j] == pixBlank &&
							 pixels[0][(h+1) * pixelsPerLine + j] != pixBlank &&
							 pixels[0][(h+1) * pixelsPerLine + j] != pixBorder
						   )
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][h * pixelsPerLine + j] = pixBorder;
							}
						}
					}

					/* ohraniceni zdola */
					if ( h > 0 && h <= result[0]->h )
					{
						if ( pixels[0][(h-1) * pixelsPerLine + j] != pixBlank &&
							 pixels[0][(h-1) * pixelsPerLine + j] != pixBorder &&
							 pixels[0][h * pixelsPerLine + j] == pixBlank
						   )
						{
							for ( Uint16 c = 0; c < CCOLORS; ++c )
							{
								pixels[c][h * pixelsPerLine + j] = pixBorder;
							}
						}
					}
				}
			}

			for (h = 0; h < result[0]->h; h++)
			{
				for (j = 0; j < result[0]->w; j++)
				{
					for ( Uint16 c = 0; c < CCOLORS; ++c )
					{
						if ( pixels[c][h * pixelsPerLine + j] == pixBorder )
						{
							if ( c == cblack )
							{
								pixels[c][h * pixelsPerLine + j] = pixC[cwhite];
							}
							else
							{
								pixels[c][h * pixelsPerLine + j] = pixC[cblack];
							}
						}
					}
				}
			}

			for ( Uint32 c = 0; c < CCOLORS; ++c )
			{
				/* ???????????????????????????????????????????????????????????????????
				if ( SDL_MUSTLOCK( result[c] ) )
					SDL_UnlockSurface ( result[c] );
				 */

				SDL_SetColorKey ( result[c], SDL_SRCCOLORKEY | SDL_RLEACCEL, pixBlank );

				this->symbols[i][c] = result[c];
			}

		}
		else
		{
			er.report( LOG_WARNING, _("%s: Letter %c (0x%x) not defined in FON file.\n"), AT, i, i );
		}

	}

	return 0;

}


SDL_Surface* CFonReader::getSymbol(Uint8 s)
{
	return symbols[s][cwhite];
}

SDL_Surface* CFonReader::getSymbol(Uint8 s, imagecolors color )
{
	return symbols[s][color];
}

Uint16 CFonReader::getSymbolWidth(Uint8 s)
{
	if ( symbols[s] != NULL )
	{
		return symbols[s][0]->w;
	}
	else
	{
		return 0;
	}
}

Uint16 CFonReader::getStringWidth(string str)
{
	Uint16 size = 0;
	for ( Uint16 i = 0; i < str.size(); ++i )
	{
		size += getSymbolWidth(str[i]);
		size += font.spacing - 2;
	}

	return size;
}

Uint16 CFonReader::getSymbolHeight()
{
	return symbols[0][0]->h;
}

Sint16 CFonReader::getSpacing()
{
	return font.spacing;
}

