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

#ifndef ANIREADER_H
#define ANIREADER_H

/**
 * -------------------- Includes
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "../def/sdl.h"
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "../global.h"
#include <debug/errorreporter.h>
// #include "render.h"
#include "filereader.h"
#include "../def/colors.h"

using namespace std;


/**
 * -------------------- Definitions
 */

/**
 * Print debug messages?
 */
// #define DEBUG

/**
 * Error messages
 */
#define ERROR_BAD_DATA "Nekorektni data!"

/**
 * Length of keywords
 */
#define KEYLEN   4
#define KEYLENEX 10

/**
 * Maximum length of sequence name
 */
#define SEQMAXNAME 42

/**
 * Two types of terminators
 */
#define HS  0 //! 0x00 0x00, HEADER + SIZE
#define HSD 1 //! 0X00 0x01, HEADER + SIZE + DATA

/**
 * How many bytes followed by a keyword that determines the size of the section
 */
#define HSIZELEN 6 //! size (4 bytes) + terminator (2 bytes)

/**
 * Bite mask for 16 bpp color
 */
#define COL16_RED       0x7c00
#define COL16_GREEN     0x03e0
#define COL16_BLUE      0x001f

/**
 * Bite mask for 8 bpp color
 */
#define COL8_RED        0xE0
#define COL8_GREEN      0x1C
#define COL8_BLUE       0x03

/**
 * Bite mask for RLE
 */
#define COL_RLE         0x80
#define COL_CNT         0x7f

/**
 * Struct of color, 32 bpp
 */
typedef struct S_RGBA
{
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;
} s_rgba;

/**
 * {{{ ANI SECTIONS
 */
typedef struct ARECT
{
    Sint16 x1;
    Sint16 y1;
    Sint16 x2;
    Sint16 y2;
    Sint16 type;
} rect;

typedef struct APOINT
{
    Sint16 x;
    Sint16 y;
    Sint16 type;
} point;

const char * const ANI_SECTIONS[] =
{
    "CHFILEANI ",
    "PAL ",
    "TPAL",
    "HEAD",
    "CBOX",
    "FRAM",
    "FNAM",
    "CIMG",
    "ATBL",
    "SEQ ",
    "STAT",
    "RECT",
    "SPRI",
    "VRAM",
    "OIMG",
    "ALSD",
    "OPAL",
    "POIN",
    "ERRR", //! Error
    "OKOK", //! Success
    "DONE"  //! EOF
};

typedef enum 
{
    EANI_CHFILEANI,
    EANI_PAL,
    EANI_TPAL,
    EANI_HEAD,
    EANI_CBOX,
    EANI_FRAM,
    EANI_FNAM,
    EANI_CIMG,
    EANI_ATBL,
    EANI_SEQ,
    EANI_STAT,
    EANI_RECT,
    EANI_SPRI,
    EANI_VRAM,
    EANI_OIMG,
    EANI_ALSD,
    EANI_OPAL,
    EANI_POIN,
    EANI_ERR, //! Error
    EANI_OK,  //! Success
    EANI_DONE //! EOF
} eani_sections;

#define EANI_SECTIONS_FIRST EANI_CHFILEANI
#define EANI_SECTIONS_LAST  EANI_POIN

typedef struct ANIHEAD
{
    Uint32 secSize;
    char * data;
} anihead;

typedef struct ANIPAL
{
    Uint32 secSize;
    char * data;
} anipal;

typedef struct ANITPAL
{
    Uint32 secSize;
    char * data;
} anitpal;

typedef struct ANICBOX
{
    Uint32 secSize;
    Sint16 cbox_w;
    Sint16 cbox_h;
} anicbox;

typedef struct ANIFILE
{
    Uint32 fileSize;
    anihead head;
    anipal pal;
    anitpal tpal;
    anicbox cbox;
} anifile;

typedef struct SEQSTATHEAD
{
    Uint32 secSize;
    Sint16 delay;
    Sint16 mx;
    Sint16 my;
    Sint16 statvar[16];
} seqstathead;

typedef struct SEQSTATRECT
{
    Uint32 secSize;
    Sint16 count;
    vector<rect> rects;
} seqstatrect;

typedef struct SEQSTATPOIN
{
    Uint32 secSize;
    Sint16 count;
    vector<point> points;

} seqstatpoin;

typedef struct SEQSTATFRAMONE
{
    Sint16 imgno;
    Sint16 relx;
    Sint16 rely;
    bool useRelPos;
    Sint16 flip;
} seqstatframone;

typedef struct SEQSTATFRAM
{
    Uint32 secSize;
    Sint16 count;
    vector<seqstatframone> frames;
} seqstatfram;

typedef struct SEQSTAT
{
    Uint32 secSize;
    seqstathead head;
    vector<seqstatrect> rects;
    vector<seqstatpoin> poins;
    vector<seqstatfram> frams;
} seqstat;

typedef struct SEQHEAD
{
    Uint32 secSize;
    string name;
    Sint16 gx;
    Sint16 gy;
    Sint16 seqvar[16];
    Sint16 states;
} seqhead;

typedef struct SEQUENCE
{
    Uint32 secSize;
    seqhead head;
    vector<seqstat> stats;
} sequence;

typedef struct CIMG
{
    Uint32 secSize;
    Sint16 type;
    Sint16 extHeader;
    bool   palHeader;
    Sint16 palSize;
    char * pal;
    Sint16 x;
    Sint16 y;
    Uint16 h;
    Uint16 w;
    Uint16 alpha;
    Sint16 bitsPerPixel;
    Uint32 compSize;
    Uint32 uncompSize;

    Sint16 unknown1;
    Uint32 unknown2;
    Sint16 unknown3;
    Sint16 unknown4;
    Sint16 unknown5;

    SDL_Surface * im[10]; // we have 10 colors
} cimg;

typedef struct FNAM
{
    Uint32 secSize;
    string name;
} fnam;

typedef struct ATBL
{
    Uint32 secSize;
    char * data;
} atbl;

typedef struct FRAMEHEAD
{
    Uint32 secSize;
} framehead;

typedef struct FRAME
{
    Uint32 secSize;
    framehead head;
    fnam file;
    cimg img;
    atbl tbl;
} frame;

/**
 * }}} ANI SECTIONS
 */

/**
 * -------------------- Classes
 */
class CAniReader
{
private:
    anifile af; /* animation file */
    vector<frame> frames; /* vector of frames */
    vector<sequence> sequences; /* vector of sequences */
    SDL_RWops * fp;
    Uint32 filesize;
    CFileReader f; /* opened animation file */
    eani_sections readKeyWord ( Uint32 bytes );
    eani_sections parseSeq();
    eani_sections parseFram();
    SDL_Surface * readImage ( Uint16 w, Uint16 h,  Uint16 alpha, Sint16 bitsPerPixel, char * pal );
    Uint8 parseFile();
    void cleanUp();

    CErrorReporter er;

public:
    CAniReader();
    ~CAniReader();
    Uint8 read ( string filename );
    Uint16 getSeqCount();
    sequence getSeq ( Uint16 index );
    vector<frame> *getFrm ();
    void printObjects();
    void prepareColor( imagecolors color );
};

#endif // ANIREADER_H
