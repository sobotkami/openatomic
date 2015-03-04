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

#include "pcxreader.h"
#include <cstdlib>

using namespace std;


CPcxReader::CPcxReader()
{
    fp = NULL;
    er.setLevel( LOG_ERROR );
}


CPcxReader::~CPcxReader()
{
    if ( fp != NULL )
        SDL_RWclose ( fp );
}


SDL_Surface * CPcxReader::read ( string filename )
{
    er.report( LOG_INFO, _("%s: Opening a file '%s'.\n"), AT, filename.c_str() );
    if ( ( fp = SDL_RWFromFile ( filename.c_str(), "rb" ) ) == NULL )
    {
        er.report( LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, filename.c_str() );
        return NULL;
    }
    else
    {
        return LoadPCX_RW ( fp );
    }
}


/* Load a PCX type image from an SDL datasource */
SDL_Surface * CPcxReader::LoadPCX_RW ( SDL_RWops *src )
{
    int start;
    struct PCXheader pcxh;
    Uint32 Rmask;
    Uint32 Gmask;
    Uint32 Bmask;
    Uint32 Amask;
    SDL_Surface *surface = NULL;
    int width, height;
    int y, bpl;
    Uint8 *row, *buf = NULL;
    const char *error = NULL;
    int bits, src_bits;

    if ( !src )
    {
        /* The error message has been set in SDL_RWFromFile */
        return NULL;
    }
    start = SDL_RWtell ( src );

    if ( ! SDL_RWread ( src, &pcxh, sizeof ( pcxh ), 1 ) )
    {
        error = _("file truncated");
        goto done;
    }
    pcxh.Xmin = SDL_SwapLE16 ( pcxh.Xmin );
    pcxh.Ymin = SDL_SwapLE16 ( pcxh.Ymin );
    pcxh.Xmax = SDL_SwapLE16 ( pcxh.Xmax );
    pcxh.Ymax = SDL_SwapLE16 ( pcxh.Ymax );
    pcxh.BytesPerLine = SDL_SwapLE16 ( pcxh.BytesPerLine );

    /* Create the surface of the appropriate type */
    width = ( pcxh.Xmax - pcxh.Xmin ) + 1;
    height = ( pcxh.Ymax - pcxh.Ymin ) + 1;
    Rmask = Gmask = Bmask = Amask = 0;
    src_bits = pcxh.BitsPerPixel * pcxh.NPlanes;
    if ( ( pcxh.BitsPerPixel == 1 && pcxh.NPlanes >= 1 && pcxh.NPlanes <= 4 )
         || ( pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 1 ) )
    {
        bits = 8;
    }
    else if ( pcxh.BitsPerPixel == 8 && pcxh.NPlanes == 3 )
    {
        bits = 24;
        if ( SDL_BYTEORDER == SDL_LIL_ENDIAN )
        {
            Rmask = 0x000000FF;
            Gmask = 0x0000FF00;
            Bmask = 0x00FF0000;
        }
        else
        {
            Rmask = 0xFF0000;
            Gmask = 0x00FF00;
            Bmask = 0x0000FF;
        }
    }
    else
    {
        error = _("unsupported PCX format");
        goto done;
    }
    surface = SDL_AllocSurface ( SDL_SWSURFACE, width, height,
                                 bits, Rmask, Gmask, Bmask, Amask );
    if ( surface == NULL )
        goto done;

    bpl = pcxh.NPlanes * pcxh.BytesPerLine;
    if ( bpl > surface->pitch )
    {
        error = _("bytes per line is too large (corrupt?)");
    }
    buf = ( Uint8 * ) malloc ( bpl );
    row = ( Uint8 * ) surface->pixels;
    for ( y=0; y<surface->h; ++y )
    {
        /* decode a scan line to a temporary buffer first */
        int i, count = 0;
        Uint8 ch;
        Uint8 *dst = ( src_bits == 8 ) ? row : buf;
        if ( pcxh.Encoding == 0 )
        {
            if ( !SDL_RWread ( src, dst, bpl, 1 ) )
            {
                error = _("file truncated");
                goto done;
            }
        }
        else
        {
            for ( i = 0; i < bpl; i++ )
            {
                if ( !count )
                {
                    if ( !SDL_RWread ( src, &ch, 1, 1 ) )
                    {
                        error = _("file truncated");
                        goto done;
                    }
                    if ( ( ch & 0xc0 ) == 0xc0 )
                    {
                        count = ch & 0x3f;
                        if ( !SDL_RWread ( src, &ch, 1, 1 ) )
                        {
                            error = _("file truncated");
                            goto done;
                        }
                    }
                    else
                        count = 1;
                }
                dst[i] = ch;
                count--;
            }
        }

        if ( src_bits <= 4 )
        {
            /* expand planes to 1 byte/pixel */
            Uint8 *src = buf;
            int plane;
            for ( plane = 0; plane < pcxh.NPlanes; plane++ )
            {
                int i, j, x = 0;
                for ( i = 0; i < pcxh.BytesPerLine; i++ )
                {
                    Uint8 byte = *src++;
                    for ( j = 7; j >= 0; j-- )
                    {
                        unsigned bit = ( byte >> j ) & 1;
                        /* skip padding bits */
                        if ( i * 8 + j >= width )
                            continue;
                        row[x++] |= bit << plane;
                    }
                }
            }
        }
        else if ( src_bits == 24 )
        {
            /* de-interlace planes */
            Uint8 *src = buf;
            int plane;
            for ( plane = 0; plane < pcxh.NPlanes; plane++ )
            {
                int x;
                dst = row + plane;
                for ( x = 0; x < width; x++ )
                {
                    *dst = *src++;
                    dst += pcxh.NPlanes;
                }
            }
        }

        row += surface->pitch;
    }

    if ( bits == 8 )
    {
        SDL_Color *colors = surface->format->palette->colors;
        int nc = 1 << src_bits;
        int i;

        surface->format->palette->ncolors = nc;
        if ( src_bits == 8 )
        {
            Uint8 ch;
            /* look for a 256-colour palette */
            do
            {
                if ( !SDL_RWread ( src, &ch, 1, 1 ) )
                {
                    error = _("file truncated");
                    goto done;
                }
            }
            while ( ch != 12 );

            for ( i = 0; i < 256; i++ )
            {
                SDL_RWread ( src, &colors[i].r, 1, 1 );
                SDL_RWread ( src, &colors[i].g, 1, 1 );
                SDL_RWread ( src, &colors[i].b, 1, 1 );
            }
        }
        else
        {
            for ( i = 0; i < nc; i++ )
            {
                colors[i].r = pcxh.Colormap[i * 3];
                colors[i].g = pcxh.Colormap[i * 3 + 1];
                colors[i].b = pcxh.Colormap[i * 3 + 2];
            }
        }
    }

done:
    free ( buf );
    if ( error )
    {
        SDL_RWseek ( src, start, RW_SEEK_SET );
        if ( surface )
        {
            SDL_FreeSurface ( surface );
            surface = NULL;
        }

        er.report( LOG_ERROR, _("%s: %s!\n"), AT, error );
    }
    return ( surface );
}
