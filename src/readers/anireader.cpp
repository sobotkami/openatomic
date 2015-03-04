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

#include "anireader.h"

using namespace std;

/**
 * Constructor
 */
CAniReader::CAniReader ()
{
    memset(&af, 0, sizeof ( af));
    er.setLevel(LOG_WARNING);
}

/**
 * Destructor
 */
CAniReader::~CAniReader ()
{
    cleanUp();
}

/**
 * Frees data and clear sequences and frames
 */
void CAniReader::cleanUp ()
{
    free(af.head.data);
    free(af.pal.data);
    free(af.tpal.data);

    size_t ngframes = frames.size();
    if (ngframes > 0)
    {
        for (size_t i = 0; i < ngframes; ++i)
        {
            free(frames[i].img.pal);
            for (size_t color = 0; color < 10; ++color)
                if (frames[i].img.im[color] != NULL)
                    SDL_FreeSurface(frames[i].img.im[color]);
        }
    }

    sequences.clear();
    frames.clear();
}

/**
 * Read keyword from file and return number of keyword.
 * @param bytes Size of keyword.
 * @return Number of keyword.
 */
eani_sections CAniReader::readKeyWord (Uint32 bytes)
{
    char * buf = NULL;

    if (f.getPos() >= (af.fileSize + KEYLENEX + HSIZELEN))
    {
        return EANI_DONE;
    }

    buf = f.readBytes(bytes);

    /* Need the \0 at the end of string */
    if ((buf = (char *) realloc(buf, bytes + 1)) == NULL)
    {
        er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, bytes + 1);
        exit(1);
    }
    buf[bytes] = '\0';

    for (int i = EANI_SECTIONS_FIRST; i <= EANI_SECTIONS_LAST; ++i)
    {
        if (strncmp(buf, ANI_SECTIONS[i], bytes) == 0)
        {
            //             er.report( LOG_INFO, _("%s: Found known keyword '%s' at %d.\n"), AT, buf, f.getPos() - bytes );
            free(buf);
            return (eani_sections) i;
        }
    }

    er.report(LOG_WARNING, _("%s: Found unknown keyword '%s' at %d.\n"), AT, buf, f.getPos() - bytes);

    free(buf);
    return EANI_ERR;
}

/**
 * Prints the retrieved structured sections and their contents.
 */
void CAniReader::printObjects ()
{
    Uint16 ngsequences = 0;
    Uint16 nstats = 0;
    Uint16 nframs = 0;
    Uint16 nframes = 0;
    Uint16 npoins = 0;
    Uint16 nrects = 0;
    Uint16 npoints = 0;
    Uint16 nrectsin = 0;

    Uint16 ngframes = 0;

    Uint16 i = 0;
    Uint16 j = 0;
    Uint16 k = 0;
    Uint16 l = 0;

    ngsequences = sequences.size();
    if (ngsequences > 0)
    {
        for (i = 0; i < ngsequences; ++i)
        {
            printf("--sequences[%d]\n", i);
            printf("  |--.head.name = '%s'\n", sequences[i].head.name.c_str());
            printf("  |--.head.g(x,y) = (%d,%d)\n", sequences[i].head.gx, sequences[i].head.gy);
            nstats = sequences[i].stats.size();
            for (j = 0; j < nstats; ++j)
            {
                printf("  |--.stats[%d]\n", j);
                printf("      |--.head.delay = %d\n", sequences[i].stats[j].head.delay);
                printf("      |--.head.m(x,y) = (%d,%d)\n", sequences[i].stats[j].head.mx, sequences[i].stats[0].head.my);

                nframs = sequences[i].stats[j].frams.size();
                for (k = 0; k < nframs; ++k)
                {
                    printf("      |--.frams[%d]\n", k);
                    nframes = sequences[i].stats[j].frams[k].frames.size();
                    for (l = 0; l < nframes; ++l)
                    {
                        printf("          |--.frames[%d].imgno = %d\n", l,
                               sequences[i].stats[j].frams[k].frames[l].imgno
                               );
                        printf("          |--.frames[%d].rel(x,y) .flip = (%d,%d) .%d\n", l,
                               sequences[i].stats[j].frams[k].frames[l].relx,
                               sequences[i].stats[j].frams[k].frames[l].rely,
                               sequences[i].stats[j].frams[k].frames[l].flip
                               );
                    }
                }

                npoins = sequences[i].stats[j].poins.size();
                for (k = 0; k < npoins; ++k)
                {
                    printf("      |--.poins[%d]\n", k);
                    npoints = sequences[i].stats[j].poins[k].points.size();
                    for (l = 0; l < npoints; ++l)
                    {
                        printf("          |--.points[%d].(x,y) .type = (%d,%d) .%d\n", l,
                               sequences[i].stats[j].poins[k].points[l].x,
                               sequences[i].stats[j].poins[k].points[l].y,
                               sequences[i].stats[j].poins[k].points[l].type
                               );
                    }
                }

                nrects = sequences[i].stats[j].rects.size();
                for (k = 0; k < nrects; ++k)
                {
                    printf("      |--.rects[%d]\n", k);
                    nrectsin = sequences[i].stats[j].rects[k].rects.size();
                    for (l = 0; l < nrectsin; ++l)
                    {
                        printf("          |--.rects[%d].(x1,y1) .(x2,y2) .type = .(%d,%d) .(%d,%d)} .%d \n", l,
                               sequences[i].stats[j].rects[k].rects[l].x1,
                               sequences[i].stats[j].rects[k].rects[l].y1,
                               sequences[i].stats[j].rects[k].rects[l].x2,
                               sequences[i].stats[j].rects[k].rects[l].x2,
                               sequences[i].stats[j].rects[k].rects[l].type
                               );
                    }
                }

            }
        }
    }

    printf("\n");

    ngframes = frames.size();
    if (ngframes > 0)
    {
        for (i = 0; i < ngframes; ++i)
        {
            printf("--frames[%d]\n", i);
            printf("  |--.file.name = '%s'\n", frames[i].file.name.c_str());
            printf("  |--.img\n");
            printf("      |--.(x,y) .(w,h) .alpha .bpp = (%d,%d) .(%d,%d) .0x%X .%d\n",
                   frames[i].img.x,
                   frames[i].img.y,
                   frames[i].img.w,
                   frames[i].img.h,
                   frames[i].img.alpha,
                   frames[i].img.bitsPerPixel
                   );
        }
    }
}

/**
 * Function open ani file, prepare to read. Then call parseFile().
 * @param filename File name of ani file.
 * @return Returns 0 if success, non-zero value if failure.
 */
Uint8 CAniReader::read (string filename)
{
    er.report(LOG_INFO, _("%s: Opening a file '%s'.\n"), AT, filename.c_str());
    if ((fp = SDL_RWFromFile(filename.c_str(), "rb")) == NULL)
    {
        er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, filename.c_str());
        return 1;
    }
    else
    {
        SDL_RWseek(fp, 0, SEEK_END);
        filesize = SDL_RWtell(fp);
        SDL_RWseek(fp, 0, SEEK_SET);

        //         struct stat fileStat;
        //         if ( fstat ( fileno ( fp->hidden.stdio.fp ),&fileStat ) < 0 )
        //         {
        //             cerr << ERROR_FILESIZE << endl;
        //             filesize = 0;
        //         }
        //         else
        //         {
        //             filesize = fileStat.st_size;
        //             cout << "filesize: " << filesize;
        //         }
        f = CFileReader(fp);

        return CAniReader::parseFile();
    }
}

/**
 * Parsing ani file
 * @return Returns 0 if success, non-zero value if failure.
 */
Uint8 CAniReader::parseFile ()
{
    eani_sections keyword;
    eani_sections res;

    /* Kontrola zacatku souboru */
    if (readKeyWord(KEYLENEX) != EANI_CHFILEANI)
    {
        er.report(LOG_ERROR, _("%s: Missing keyword '%s' at %d!\n"), AT, "CHFILEANI", f.getPos() - KEYLENEX);
        return 1;
    }
    af.fileSize = f.read32();

    if (f.read16() != HS)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return 1;
    }

    if (af.fileSize + KEYLENEX + HSIZELEN != filesize)
    {
        er.report(LOG_ERROR, _("%s: The file is shorter than it should be (%d bytes vs %d bytes)!\n"), AT, af.fileSize + KEYLENEX + HSIZELEN, filesize);
        return 1;
    }
    else
    {
        er.report(LOG_INFO, _("%s: File size is %d bytes, file size from main header is %d bytes.\n"), AT, filesize, af.fileSize);
    }

    /* nacteni globalni hlavicky */
    if (readKeyWord(KEYLEN) != EANI_HEAD)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "HEAD", f.getPos() - KEYLEN);
        return 1;
    }

    af.head.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return 1;
    }

    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "HEAD", af.head.secSize);
    af.head.data = f.readBytes(af.head.secSize);

    er.report(LOG_INFO, _("%s: Skipping '%s' data.\n"), AT, "HEAD");

    // #ifdef DEBUG
    //     printHex ( af.head.data, af.head.secSize, HEXNEWLINE );
    // #endif

    /* Kontrola hlavicky PAL */
    if (readKeyWord(KEYLEN) != EANI_PAL)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "PAL", f.getPos() - KEYLEN);
        return 1;
    }
    af.pal.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return 1;
    }

    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "PAL", af.pal.secSize);
    af.pal.data = f.readBytes(af.pal.secSize);

    er.report(LOG_INFO, _("%s: Skipping '%s' data.\n"), AT, "PAL");


    /* Kontrola hlavicky TPAL */
    if (readKeyWord(KEYLEN) != EANI_TPAL)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "TPAL", f.getPos() - KEYLEN);
        return 1;
    }
    af.tpal.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return 1;
    }

    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "TPAL", af.tpal.secSize);
    af.tpal.data = f.readBytes(af.tpal.secSize);

    er.report(LOG_INFO, _("%s: Skipping '%s' data.\n"), AT, "TPAL");


    /* Kontrola hlavicky CBOX */
    if (readKeyWord(KEYLEN) != EANI_CBOX)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "CBOX", f.getPos() - KEYLEN);
        return 1;
    }
    af.cbox.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return 1;
    }

    if (af.cbox.secSize != 4)
    {
        er.report(LOG_WARNING, _("%s: The size of the section '%s' is not %d bytes!\n"), AT, "CBOX", 4);
        return 1;
    }
    else
    {
        er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "CBOX", af.cbox.secSize);
    }

    af.cbox.cbox_w = f.read16();
    af.cbox.cbox_h = f.read16();
    er.report(LOG_INFO, _("%s: CBOX w:h = %d:%d pixels.\n"), AT, af.cbox.cbox_w, af.cbox.cbox_h);

    er.report(LOG_INFO, _("%s: Loading dynamic section...\n"), AT);

    while (5)
    {
        keyword = readKeyWord(KEYLEN);
        switch (keyword)
        {
        case EANI_SEQ:
            res = parseSeq();
            if (res == EANI_DONE)
                return 0;
            if (res == EANI_ERR)
                return 1;
            break;
        case EANI_FRAM:
            res = parseFram();
            if (res == EANI_DONE)
                return 0;
            if (res == EANI_ERR)
                return 1;
            break;
        case EANI_ERR:
            er.report(LOG_ERROR, _("%s: Incorrect data!\n"), AT);
            return 1;
            break;
        case EANI_DONE:
            return 0;
            break;
        default:
            er.report(LOG_INFO, _("%s: Next section is '%s'.\n"), AT, ANI_SECTIONS[keyword]);
            break;
        }

    }

    SDL_RWclose(fp);
    return 0;
}

/**
 * Parse SEQ section from ani file.
 * @return Returns EANI_OK if success, EANI_DONE if become EOF, else EANI_ERR.
 */
eani_sections CAniReader::parseSeq ()
{
    rect r;
    point p;
    seqstatrect statr;
    seqstatpoin statp;
    seqstatfram statf;
    seqstatframone statfo;
    seqstat stat;
    sequence seq;
    eani_sections keyword = EANI_OK;

    char * seqname = NULL;
    int i = 0;
    int j = 0;

    seq.secSize = f.read32();
    if (f.read16() != HS)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return EANI_ERR;
    }
    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "SEQ", seq.secSize);

    if (readKeyWord(KEYLEN) != EANI_HEAD)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "HEAD", f.getPos() - KEYLEN);
        return EANI_ERR;
    }

    seq.head.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return EANI_ERR;
    }
    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "HEAD", seq.head.secSize);

    seqname = f.readBytes(SEQMAXNAME);
    seq.head.name.assign(seqname);
    free(seqname);
    er.report(LOG_INFO, _("%s: Sequence name is '%s'.\n"), AT, seq.head.name.c_str());


    seq.head.gx = f.read16();
    seq.head.gy = f.read16();

    er.report(LOG_INFO, _("%s: Global origin x:y = %d:%d\n"), AT, seq.head.gx, seq.head.gy);

    for (i = 0; i < 16; i++)
    {
        seq.head.seqvar[i] = f.read16();
        er.report(LOG_INFO, _("%s: Sequence variable %2d: %d\n"), AT, i, seq.head.seqvar[i]);
    }

    seq.head.states = f.read16();
    er.report(LOG_INFO, _("%s: Number of sequence states is %d.\n"), AT, seq.head.states);

    er.report(LOG_INFO, _("%s: --> Unknown %d bytes at %d - skipping.\n"), AT, 16, f.getPos());
    f.setRelPos(16); /* unknown 16 bytes */

    for (i = 0; i < seq.head.states; i++)
    {
        er.report(LOG_INFO, _("%s: Loading %d. state of sequence.\n"), AT, i + 1);
        if (readKeyWord(KEYLEN) != EANI_STAT)
        {
            er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "STAT", f.getPos() - KEYLEN);
            return EANI_ERR;
        }

        stat.rects.clear();
        stat.poins.clear();
        stat.frams.clear();

        stat.secSize = f.read32();
        if (f.read16() != HS)
        {
            er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
            return EANI_ERR;
        }
        er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "STAT", stat.secSize);

        if (readKeyWord(KEYLEN) != EANI_HEAD)
        {
            er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "HEAD", f.getPos() - KEYLEN);

            return EANI_ERR;
        }

        stat.head.secSize = f.read32();
        if (f.read16() != HSD)
        {
            er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
            return EANI_ERR;
        }
        er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "HEAD", stat.head.secSize);

        stat.head.delay = f.read16();
        er.report(LOG_INFO, _("%s: Frame duration is %d ms\n"), AT, stat.head.delay);

        stat.head.mx = f.read16();
        stat.head.my = f.read16();

        er.report(LOG_INFO, _("%s: Shift axis x:y = %d:%d\n"), AT, stat.head.mx, stat.head.my);

        for (j = 0; j < 16; j++)
        {
            stat.head.statvar[j] = f.read16();
            er.report(LOG_INFO, _("%s: State variable %2d: %d\n"), AT, j, stat.head.statvar[j]);
        }

        er.report(LOG_INFO, _("%s: --> Unknown %d bytes at %d - skipping.\n"), AT, 8, f.getPos());
        f.setRelPos(8); /* unknown 8 bytes */

        while (5)
        {
            keyword = readKeyWord(KEYLEN);
            if (keyword == EANI_RECT)
            {
                stat.rects.clear();
                statr.secSize = f.read32();
                if (f.read16() != HSD)
                {
                    er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
                    return EANI_ERR;
                }

                er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "RECT", statr.secSize);

                statr.count = f.read16();
                er.report(LOG_INFO, _("%s: Number of rectangles is %d.\n"), AT, statr.count);

                for (j = 0; j < statr.count; j++)
                {
                    r.x1 = f.read16();
                    r.y1 = f.read16();
                    r.x2 = f.read16();
                    r.y2 = f.read16();
                    r.type = f.read16();
                    statr.rects.push_back(r);

                    er.report(LOG_INFO, _("%s: Rectangle type %d x1,y1:x2,y2 = %d,%d:%d,%d\n"), AT, r.type, r.x1, r.y1, r.x2, r.y2);
                }

                stat.rects.push_back(statr);
            }
            else if (keyword == EANI_POIN)
            {
                stat.poins.clear();
                statp.secSize = f.read32();
                if (f.read16() != HSD)
                {
                    er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
                    return EANI_ERR;
                }

                er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "POIN", statp.secSize);

                statp.count = f.read16();
                er.report(LOG_INFO, _("%s: Number of points is %d.\n"), AT, statp.count);

                for (j = 0; j < statp.count; j++)
                {
                    p.x = f.read16();
                    p.y = f.read16();
                    p.type = f.read16();
                    statp.points.push_back(p);

                    er.report(LOG_INFO, _("%s: Point type %d x:y = %d:%d\n"), AT, p.type, p.x, p.y);
                }

                stat.poins.push_back(statp);
            }
            else if (keyword == EANI_FRAM)
            {
                statf.frames.clear();

                statf.secSize = f.read32();
                if (f.read16() != HSD)
                {
                    er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
                    return EANI_ERR;
                }

                er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "FRAM", statf.secSize);


                statf.count = f.read16();
                er.report(LOG_INFO, _("%s: Number of frames is %d.\n"), AT, statf.count);

                for (j = 0; j < statf.count; j++)
                {
                    er.report(LOG_INFO, _("%s: Frame %d:\n"), AT, j + 1);

                    statfo.imgno = f.read16();
                    er.report(LOG_INFO, _("%s: Used image number is %d\n"), AT, statfo.imgno);

                    statfo.relx = f.read16();
                    statfo.rely = f.read16();

                    er.report(LOG_INFO, _("%s: Relative shift of axes x:y = %d:%d\n"), AT, statfo.relx, statfo.rely);

                    /* HACK! animace ohne vyzaduje relativni souradnice */
                    if (seq.head.name.substr(0, 5) == "flame")
                    {
                        statfo.useRelPos = true;
                    }
                    else
                    {
                        statfo.useRelPos = false;
                    }

                    statfo.flip = f.read16();
                    er.report(LOG_INFO, _("%s: Frame is reflected? %d\n"), AT, statfo.flip);

                    short int unknownnum;
                    unknownnum = f.read16();

                    if (unknownnum == 0) /* often 0 ... */
                    {
                        er.report(LOG_INFO, _("%s: Unknown value %d: %d\n"), AT, 1, unknownnum);
                    }
                    else
                    {
                        er.report(LOG_WARNING, _("%s: Unknown value %d: %d\n"), AT, 1, unknownnum);
                    }

                    statf.frames.push_back(statfo);
                }

                stat.frams.push_back(statf);
            }
            else if (keyword == EANI_DONE)
            {
                break;
            }
            else
            {
                //                 er.report( LOG_INFO, _("%s: Next section is '%s', end loading section '%s'\n"), AT, "STAT", ANI_SECTIONS[keyword] );

                f.setRelPos(-KEYLEN);
                break;
            }
        }

        seq.stats.push_back(stat);
    }

    sequences.push_back(seq);

    if (keyword == EANI_DONE)
        return EANI_DONE; // konec souboru

    return EANI_OK;
}

/**
 * Parse FRAM section from ani file and prepare images -- loads to SDL_Surface
 * @return Returns EANI_OK if success, EANI_DONE if become EOF, else EANI_ERR.
 */
eani_sections CAniReader::parseFram ()
{

    frame frm;
    eani_sections keyword = EANI_ERR;
    int rBytes = 0;
    s_rgba pix_p;
    SDL_PixelFormat * pxlformat = NULL;
    
    if (SDL_GetVideoSurface() == NULL)
    {
        er.report(LOG_ERROR, _("%s: SDL_GetVideoSurface() fail. Before SDL_Init()?\n"), AT);
        return EANI_ERR;
    }
    
    pxlformat = SDL_GetVideoSurface()->format;

    // we have 10 colors
    for (int color = 0; color < 10; ++color)
    {
        frm.img.im[color] = NULL;
    }
    frm.img.pal = NULL;

    frm.secSize = f.read32();
    if (f.read16() != HS)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
        return EANI_ERR;
    }

    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "FRAM", frm.secSize);


    if (readKeyWord(KEYLEN) != EANI_HEAD)
    {
        er.report(LOG_ERROR, _("%s: Missing section '%s' at %d!\n"), AT, "HEAD", f.getPos() - KEYLEN);
        return EANI_ERR;
    }

    frm.head.secSize = f.read32();
    if (f.read16() != HSD)
    {
        er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);

        return EANI_ERR;
    }

    er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "HEAD", frm.head.secSize);

    f.read16(); /* unknown number */

    while (5)
    {
        keyword = readKeyWord(KEYLEN);
        if (keyword == EANI_FNAM)
        {
            frm.file.secSize = f.read32();
            if (f.read16() != HSD)
            {
                er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);
                return EANI_ERR;
            }

            char * tmpname = f.readBytes(frm.file.secSize);
            frm.file.name.append(tmpname, frm.file.secSize);
            free(tmpname);

            er.report(LOG_INFO, _("%s: The file name is %d bytes long and contains: '%s'.\n"), AT, frm.file.secSize, frm.file.name.c_str());
        }
        else if (keyword == EANI_CIMG)
        {
            frm.img.secSize = f.read32();
            if (f.read16() != HSD)
            {
                er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);

                return EANI_ERR;
            }
            er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "CIMG", frm.img.secSize);

            long tmpSeek = f.getPos();

            frm.img.palHeader = false;
            frm.img.palSize = 0;
            rBytes = 0;

            /* counting readed bytes */

            frm.img.type = f.read16();
            frm.img.unknown1 = f.read16(); // printf()?
            frm.img.extHeader = f.read32();

            rBytes += 8;

            if (frm.img.extHeader < 24)
            {
                er.report(LOG_ERROR, _("%s: Header is short (less than 24 bytes)!\n"), AT);
                return EANI_ERR;
            }

            if (frm.img.extHeader >= 32)
            {
                frm.img.palHeader = true;
            }

            if (frm.img.extHeader > 32)
            {
                frm.img.palSize = (frm.img.extHeader - 32);

                er.report(LOG_INFO, _("%s: Image contains color palette of %d bytes.\n"), AT, frm.img.palSize);

            }

            if (frm.img.extHeader == 32)
            {
                er.report(LOG_ERROR, _("%s: Image contains color palette, but it is empty!\n"), AT);

                return EANI_ERR;
            }

            //          printHex( readBytes(frm.img.unknown1), frm.img.unknown1, HEXNEWLINE );
            frm.img.unknown2 = f.read32(); // printf()?

            rBytes += 4;

            if ((!frm.img.palHeader && frm.img.unknown2 != 0) || (frm.img.palHeader && frm.img.unknown2 != 24))
            {
                er.report(LOG_WARNING, _("%s: frm.img.unknown2 = %d\n"), AT, frm.img.unknown2);
            }


            frm.img.w = f.read16();
            frm.img.h = f.read16();
            frm.img.x = f.read16();
            frm.img.y = f.read16();
            frm.img.alpha = f.read16();
            frm.img.unknown3 = f.read16(); // printf()?

            /* convert from 15bpp to 16bpp */
            pix_p.red = ((frm.img.alpha & COL16_RED) >> 7);
            pix_p.green = ((frm.img.alpha & COL16_GREEN) >> 2);
            pix_p.blue = ((frm.img.alpha & COL16_BLUE) << 3);
            frm.img.alpha = SDL_MapRGB(pxlformat, pix_p.red, pix_p.green, pix_p.blue);

            rBytes += 12;

            switch (frm.img.type)
            {
            case 0x04:
                frm.img.bitsPerPixel = 16;
                break;
            case 0x05:
                frm.img.bitsPerPixel = 24;
                if (frm.img.palSize > 0)
                {
                    er.report(LOG_ERROR, _("%s: Image with 24 bits per pixel should not contain palette, but it contains!"), AT);

                    return EANI_ERR;
                }


                break;
            case 0x0A:
                frm.img.bitsPerPixel = 4;
                if (frm.img.palSize != 64)
                {
                    er.report(LOG_ERROR, _("%s: Image contains %d bits per pixel color palette, but is too short (%d)!\n"), AT, 4, frm.img.palSize);

                    return EANI_ERR;
                }

                break;
            case 0x0B:
                frm.img.bitsPerPixel = 8;
                if (frm.img.palSize != 1024)
                {
                    er.report(LOG_ERROR, _("%s: Image contains %d bits per pixel color palette, but is too short (%d)!\n"), AT, 8, frm.img.palSize);

                    return EANI_ERR;
                }

                break;
            default:
                er.report(LOG_ERROR, _("%s: Unknown image type!"), AT);

                return EANI_ERR;
            }

            if (frm.img.palHeader)
            {
                unsigned int v1, v2;
                v1 = f.read32(); // printf()?
                v2 = f.read32(); // printf()?

                rBytes += 8;

                if (v1 != 0x1000000 && v1 != 0x100000)
                {
                    er.report(LOG_WARNING, _("%s: v1 = %d\n"), AT, v1);
                }

                if (v2 != 8)
                {
                    er.report(LOG_WARNING, _("%s: v2 = %d\n"), AT, v2);
                }
            }

            if (frm.img.palSize > 0)
            {
                frm.img.pal = f.readBytes(frm.img.palSize);

                rBytes += frm.img.palSize;

                // #ifdef DEBUG
                //                 printf ( "  Paleta (dump): \n" );
                //                 printHex ( frm.img.pal, frm.img.palSize, HEXNEWLINE );
                // #endif
            }

            frm.img.unknown4 = f.read16(); // printf()?
            frm.img.unknown5 = f.read16(); // printf()?

            rBytes += 4;

            if (frm.img.unknown4 != 0x10 && frm.img.unknown4 != 0x11 && frm.img.unknown4 != 0x12)
                er.report(LOG_WARNING, _("%s: frm.img.unknown4 = %d\n"), AT, frm.img.unknown4);

            if (frm.img.unknown5 != 12)
                er.report(LOG_WARNING, _("%s: frm.img.unknown5 = %d\n"), AT, frm.img.unknown5);


            frm.img.compSize = f.read32() - frm.img.unknown5; // - 12
            frm.img.uncompSize = f.read32();

            rBytes += 8;


            er.report(LOG_INFO, _("%s: CIMG w:h = %d:%d\n"), AT, frm.img.w, frm.img.h);
            er.report(LOG_INFO, _("%s: CIMG x:y = %d:%d\n"), AT, frm.img.x, frm.img.y);
            er.report(LOG_INFO, _("%s: Alpha 0x%04x\n"), AT, frm.img.alpha);
            er.report(LOG_INFO, _("%s: Bits per pixel: %d\n"), AT, frm.img.bitsPerPixel);
            er.report(LOG_INFO, _("%s: Image size compressed: %d\n"), AT, frm.img.compSize);
            er.report(LOG_INFO, _("%s: Image size uncompressed: %d\n"), AT, frm.img.uncompSize);

            // Green color is default color stored in ANI files, so we saved it.
            frm.img.im[cgreen] = readImage(frm.img.w, frm.img.h, frm.img.alpha, frm.img.bitsPerPixel, frm.img.pal);

            //             SDL_Surface * NewSurface = SDL_DisplayFormatAlpha ( frm.img.im );
            //             SDL_FreeSurface ( frm.img.im );
            //             frm.img.im = NewSurface;

            if (!f.testByte(0xff))
            {
                f.setRelPos(tmpSeek + (frm.img.compSize + rBytes) - 1);
                er.report(LOG_WARNING, _("%s: Setting new position in file at %ld!\n"), AT, (long int) f.getPos());

                if (!f.testByte(0xff))
                {
                    er.report(LOG_ERROR, _("%s: Incorrect terminator (excepted byte 0xff)!"), AT);
                    return EANI_ERR;
                    //                     exit ( 5 ); //??
                }
            }

            frames.push_back(frm);
        }
        else if (keyword == EANI_ATBL)
        {
            frm.tbl.secSize = f.read32();
            if (f.read16() != HSD)
            {
                er.report(LOG_ERROR, _("%s: Wrong section separator at %d!\n"), AT, f.getPos() - 2);

                return EANI_ERR;
            }

            er.report(LOG_INFO, _("%s: The size of the section '%s' is %d bytes.\n"), AT, "ATBL", frm.tbl.secSize);

            frm.tbl.data = f.readBytes(frm.tbl.secSize);

            er.report(LOG_INFO, _("%s: Skipping '%s' data.\n"), AT, "ATBL");
        }
        else if (keyword == EANI_DONE)
        {
            break;
        }
        else
        {
            // #ifdef DEBUG
            //             printf ( "Nasleduje sekce %s -- koncim nacitani FRAM\n", ANI_SECTIONS[keyword] );
            // #endif
            f.setRelPos(-KEYLEN);
            break;
        }
    }

    return EANI_OK;

}

SDL_Surface* CAniReader::readImage (Uint16 w, Uint16 h, Uint16 alpha, Sint16 bitsPerPixel, char* pal)
{

    Uint16 pix16 = 0;
    Uint8 pix8 = 0;
    Uint16 offset = 0;
    Uint8 rle = 0;
    Uint32 counter = 0;
    Uint32 i = 0;
    Uint32 j = 0;
    s_rgba pix_p;
    SDL_Surface *result;
    SDL_PixelFormat * pxlformat = NULL;
    Uint16 *pixels = NULL;
    //Uint8 bypp = 0;

    if (SDL_GetVideoSurface() == NULL)
    {
        er.report(LOG_ERROR, _("%s: SDL_GetVideoSurface() fail. Before SDL_Init()?\n"), AT);
        return NULL;
    }

    pxlformat = SDL_GetVideoSurface()->format;

    result = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                  w, h,
                                  pxlformat->BitsPerPixel,
                                  pxlformat->Rmask,
                                  pxlformat->Gmask,
                                  pxlformat->Bmask,
                                  pxlformat->Amask
                                  );

    //bypp = result->format->BytesPerPixel;

    if (result == NULL)
    {
        er.report(LOG_ERROR, _("%s: CreateRGBSurface(): %s.\n"), AT, SDL_GetError());
        return NULL;
    }

    //     pix_p.red = ( (alpha & COL16_RED) >> 7 );
    //     pix_p.green = ( (alpha & COL16_GREEN) >> 2 );
    //     pix_p.blue = ( (alpha & COL16_BLUE) << 3 );
    //     alpha = SDL_MapRGB ( pxlformat, pix_p.red, pix_p.green, pix_p.blue );

    /* ???????????????????????????????????????????????????????????????????
    if (SDL_MUSTLOCK(result))
        SDL_LockSurface(result);
     */
    for (i = 0; i < (Uint32) (w * h);)
    {
        rle = f.read8();

        counter = (rle & COL_CNT) + 1; /* pocet pixelu */

        if (rle & COL_RLE)
        { /* Je pouzito RLE */
            /* nacteme barvu, ktera se bude opakovat */
            switch (bitsPerPixel)
            {
            case 16: /* cte se barva z disku, provadi se posuny */
                pix16 = f.read16();
                pix_p.red = ((pix16 & COL16_RED) >> 7);
                pix_p.green = ((pix16 & COL16_GREEN) >> 2);
                pix_p.blue = ((pix16 & COL16_BLUE) << 3);
                pix16 = SDL_MapRGB(pxlformat, pix_p.red, pix_p.green, pix_p.blue);
                break;
            case 8: /* cte se offset z disku, barva se hleda v palete */
                pix8 = f.read8();
                offset = pix8 << 2; /* offset vynasobit 4 */
                pix_p.red = pal[offset++];
                pix_p.green = pal[offset++];
                pix_p.blue = pal[offset++];
                pix_p.alpha = pal[offset];
                pix16 = SDL_MapRGBA(pxlformat, pix_p.red, pix_p.green, pix_p.blue, pix_p.alpha);
                break;
            default:
                er.report(LOG_ERROR, _("%s: There is no implementation for images with %d bits per pixel!\n"), AT, bitsPerPixel);
                break;
            }
            for (j = 0; j < counter; j++, i++)
            { /* zapis FB, counterkrat stejna barva */
                pixels = (Uint16 *) result->pixels + (i / w) * result->pitch / sizeof ( Uint16) + (i % w);
                *pixels = pix16;
            }
        }
        else
        {

            for (j = 0; j < counter; j++, i++)
            { /* zapis FB, counterkrat jina barva */
                /* nacteme barvu */
                switch (bitsPerPixel)
                {
                case 16:
                    pix16 = f.read16();
                    pix_p.red = ((pix16 & COL16_RED) >> 7);
                    pix_p.green = ((pix16 & COL16_GREEN) >> 2);
                    pix_p.blue = ((pix16 & COL16_BLUE) << 3);
                    pix16 = SDL_MapRGB(pxlformat, pix_p.red, pix_p.green, pix_p.blue);
                    break;
                case 8:
                    pix8 = f.read8();
                    offset = pix8 << 2; /* offset vynasobit 4 */
                    pix_p.red = pal[offset++];
                    pix_p.green = pal[offset++];
                    pix_p.blue = pal[offset++];
                    pix_p.alpha = pal[offset];
                    pix16 = SDL_MapRGBA(pxlformat, pix_p.red, pix_p.green, pix_p.blue, pix_p.alpha);
                    break;
                default:
                    er.report(LOG_ERROR, _("%s: There is no implementation for images with %d bits per pixel!\n"), AT, bitsPerPixel);
                    break;
                }
                pixels = (Uint16 *) result->pixels + (i / w) * result->pitch / sizeof ( Uint16) + (i % w);
                *pixels = pix16;
            }
        }
    }

    /* ???????????????????????????????????????????????????????????????????
    if (SDL_MUSTLOCK(result))
        SDL_UnlockSurface(result);
     */

    SDL_SetColorKey(result, SDL_SRCCOLORKEY | SDL_RLEACCEL, alpha);

    //     printf("%d bpp -->", result->format->BitsPerPixel );
    //     SDL_Surface *result_convert = SDL_ConvertSurface ( result, pxlformat, SDL_HWSURFACE | SDL_RLEACCEL );
    //     printf(" %d bpp\n", result_convert->format->BitsPerPixel );

    //     SDL_BlitSurface ( result, NULL, result, NULL );
    //     SDL_FreeSurface ( result );


    SDL_Surface * result_convert = SDL_DisplayFormat(result);
    SDL_FreeSurface(result);


    /* OBSOLETE! */
    //     if ( debug )
    //     {
    //         render.drawObject ( result, 0, 0, false );
    //     }

    return result_convert;
}

void CAniReader::prepareColor (imagecolors color)
{
    Uint16 ngframes = frames.size();
    Uint16 npixels = 0;
    Uint16 i = 0;
    Uint8 r, g, b, a;
    SDL_PixelFormat * pxlformat = NULL;

    if (color == cgreen) /* green is default, skip */
        return;

    
    if (SDL_GetVideoSurface() == NULL)
    {
        er.report(LOG_ERROR, _("%s: SDL_GetVideoSurface() fail. Before SDL_Init()?\n"), AT);
        return;
    }
    
    pxlformat = SDL_GetVideoSurface()->format;

    if (ngframes > 0)
    {
        for (i = 0; i < ngframes; ++i)
        {
            // this color is prepared, skip
            if (frames[i].img.im[color] != NULL)
            {
                continue;
            }

            npixels = frames[i].img.im[cgreen]->w * frames[i].img.im[cgreen]->h;

            frames[i].img.im[color] = SDL_ConvertSurface(frames[i].img.im[cgreen], pxlformat, SDL_HWSURFACE);

            /* ???????????????????????????????????????????????????????????????????
            if (SDL_MUSTLOCK(frames[i].img.im[cgreen]))
                SDL_LockSurface(frames[i].img.im[cgreen]);

            if (SDL_MUSTLOCK(frames[i].img.im[color]))
                SDL_LockSurface(frames[i].img.im[color]);
             */

            for (Uint16 p = 0; p < npixels; ++p)
            {
                SDL_GetRGBA(*((Uint16 *) (frames[i].img.im[cgreen]->pixels) + p), pxlformat, &r, &g, &b, &a);
                if (frames[i].img.alpha != *((Uint16 *) (frames[i].img.im[cgreen]->pixels) + p) && g > r && g > b)
                {
                    switch (color)
                    {
                    default:
                    case cgreen:
                        break;
                    case cyellow:
                        r = g;
                        break;
                    case ccyan:
                        b = g;
                        break;
                    case cred:
                        r = g;
                        g = 0;
                        break;
                    case cblue:
                        b = g;
                        g = 0;
                        break;
                    case corange:
                        r = g;
                        g = (Uint8) (r / 1.5);
                        break;
                    case cmagenta:
                        b = g;
                        r = g;
                        g = 0;
                        break;
                    case cpurple:
                        b = g;
                        r = (Uint8) (g / 1.5);
                        g = 0;
                        break;
                    case cwhite:
                        r = g;
                        b = g;
                        break;
                    case cblack:
                        g = g / 10;
                        r = g;
                        b = (Uint8) (g / 1.5);
                        break;
                    case cbrown:
                        r = g;
                        g = r << 2;
                        r = r << 1;
                        b = b << 1;
                    }
                }

                *((Uint16 *) (frames[i].img.im[color]->pixels) + p) = (Uint16) SDL_MapRGBA(pxlformat, r, g, b, a);

            }

            /* ???????????????????????????????????????????????????????????????????
            if (SDL_MUSTLOCK(frames[i].img.im[cgreen]))
                SDL_UnlockSurface(frames[i].img.im[cgreen]);

            if (SDL_MUSTLOCK(frames[i].img.im[color]))
                SDL_UnlockSurface(frames[i].img.im[color]);
             */

            //#ifndef NDEBUG
            //            render.drawObject ( frames[i].img.im[color], 0, 0, false );
            //#endif
        }
    }
}

/**
 * Returns the number of loaded sequences.
 * @return Number of sequences..
 */
Uint16 CAniReader::getSeqCount ()
{
    return sequences.size();
}

/**
 * Returns the sequence, an indexed parameter. Numbered from 0.
 * @param index Index of sequence
 * @return Returns sequence.
 */
sequence CAniReader::getSeq (Uint16 index)
{
    return sequences[index];
}

vector<frame> * CAniReader::getFrm ()
{
    return &frames;
}


