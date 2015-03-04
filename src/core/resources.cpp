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
 * File:   resources.cpp
 * Author: minji
 *
 * Created on 5. Januar 2013, 20:37
 */

#include <core/resources.h>
#include <def/sdl.h>
#include <def/files.h>
#include <def/screen.h>

//#include <cstdio>

#include <render/stringdraw.h>
#include <SDL/SDL_mixer.h>

CResources *CResources::s_instance;

CAnimation a_walk[CDIRECTIONS];
CAnimation a_stand[CDIRECTIONS];
CAnimation a_kick[CDIRECTIONS];
CAnimation a_punch[CDIRECTIONS];
CAnimation a_pickup[CDIRECTIONS];
CAnimation a_spin;

CAnimation a_shadow;
CAnimation a_hurry;
CAnimation a_xxx;
CAnimation a_cursor1;
CAnimation a_ring;

CAnimation a_power[CPOWERUPSR];
CAnimation a_bomb[4];
CAnimation a_cornerhead[CCORNERHEAD];
CAnimation a_die[CDIES];
CAnimation a_flame[CFIRES]; /* flame */
CAnimation a_flameb[CFIELDS]; /* brick */
CAnimation a_tile[CFIELDS][CTILES];

CAnimation a_arrow[CDIRECTIONS];
CAnimation a_warp;
CAnimation a_trampoline;
CAnimation a_conveyor[CDIRECTIONS];

CAnimation a_font;

CGameObject tiles[CFIELDS][CTILES];
CGameObject arrows[CDIRECTIONS];
CGameObject trampolines;
CGameObject warps;
CGameObject conveyors[CDIRECTIONS];
CGameObject bombwalk;
CGameObject powerups[CPOWERUPSR];
CGameObject fires[CFIRES];
CGameObject firesb[CFIELDS];

SDL_Surface * b_field[CFIELDS];

Mix_Chunk* s_chooseSound;
Mix_Chunk* s_dieSounds[C_DIE_SOUNDS];
Mix_Chunk* s_explodeSounds[C_EXPLODE_SOUNDS];

SDL_Thread* resThread;

#define MAX_BUFFER 512

CResources::CResources ()
{
    er.setLevel(LOG_INFO);
    m_loaded = false;

    for (Uint16 i = 0; i < CFIELDS; ++i)
    {
        b_field[i] = NULL;
    }

    // threaded load resources
    resThread = SDL_CreateThread(staticResThreadWorker, this);

    er.report(LOG_INFO, _("%s: CResources()\n"), AT);
}

CResources::~CResources ()
{
    for (Uint16 i = 0; i < CFIELDS; ++i)
    {
        if (b_field[i] != NULL)
        {
            SDL_FreeSurface(b_field[i]);
        }
    }

    for (Uint16 i = 0; i < C_DIE_SOUNDS; i++)
    {
        Mix_FreeChunk(s_dieSounds[i]);
    }

    for (Uint16 i = 0; i < C_EXPLODE_SOUNDS; i++)
    {
        Mix_FreeChunk(s_explodeSounds[i]);
    }

    Mix_FreeChunk(s_chooseSound);
}

bool CResources::Load ()
{
    er.report(LOG_INFO, _("%s: Loading resources ...\n"), AT);

    if (!LoadPcx())
    {
        return false;
    }

    CAliReader ali;

    if (!LoadAli(ali))
    {
        return false;
    }

    if (!LoadAni(ali))
    {
        return false;
    }

    SetupAni();

    if(!loadSounds())
    {
        return false;
    }

    // All ok
    m_loaded = true;
    return m_loaded;
}

bool CResources::LoadAli (CAliReader &ali)
{
    if (ali.read(PATH_FILE_MASTERALI))
    {
        er.report(LOG_ERROR, _("%s: Unable parse file '%s'!\n"), AT, PATH_FILE_MASTERALI);
        return false;
    }
    er.report(LOG_INFO, _("%s: In ALI file '%s' are %d items.\n"), AT, PATH_FILE_MASTERALI, ali.getCount());
    return true;
}

bool CResources::LoadAni (CAliReader &ali)
{
    Uint16 anifiles = ali.getCount();

    CSeqParser seq;

    CAniReader *anis;
    anis = new CAniReader[anifiles];

    char buffer [MAX_BUFFER] = "";

    ostringstream stm;

    for (Uint16 i = 0; i < anifiles; ++i)
    {

        snprintf(buffer, MAX_BUFFER, _("(%d/%d) DATA/ANI/%s"), i, anifiles - 1, ali.getItem(i).c_str());
        m_statusString.assign(buffer);

        stm.str("");
        stm << PATH_DIR_ANI << ali.getItem(i);
        if (anis[i].read( stm.str() ))
        {
            delete[] anis;
            er.report(LOG_ERROR, _("%s: Unable parse file '%s'!\n"), AT, ali.getItem(i).c_str());
            return false;
        }
        else
        {
            er.report(LOG_INFO, _("%s: Parse file '%s'.\n"), AT, ali.getItem(i).c_str());
        }

        Uint16 seqs = anis[i].getSeqCount();
        for (Uint16 a = 0; a < seqs; ++a)
        {
            //             cout << "(*(anis[i].getFrm())).[]: " << (*(anis[i].getFrm()))[0].file.name << endl;

            switch (seq.assign(anis[i].getSeq(a), &(*(anis[i].getFrm()))))
            {
            case PREPARE:
                anis[i].prepareColor(cyellow);
                anis[i].prepareColor(ccyan);
                anis[i].prepareColor(cred);
                anis[i].prepareColor(cblue);
                anis[i].prepareColor(corange);
                anis[i].prepareColor(cmagenta);
                anis[i].prepareColor(cpurple);
                anis[i].prepareColor(cwhite);
                anis[i].prepareColor(cblack);
                break;
            case NOTPREPARE:
                break;
            };
        }
    }

    return true;
}

void CResources::SetupAni ()
{
    // tiles
    for (Uint8 i = 0; i < CFIELDS; ++i)
    {
        firesb[i].setArea(ea_gameBoard);
        firesb[i].setType(GOT_FIRESB);
        firesb[i].setAni(&(a_flameb[i]));
        firesb[i].startAnim();

        for (Uint8 j = 0; j < CTILES; ++j)
        {
            tiles[i][j].setArea(ea_gameBoard);
            tiles[i][j].setType(GOT_BRICK);
            tiles[i][j].setAni(&(a_tile[i][j]));
        }
    }

    // arrows + conveyors
    for (Uint8 i = 0; i < CDIRECTIONS; ++i)
    {
        arrows[i].setArea(ea_gameBoard);
        arrows[i].setType(GOT_ARROW);
        arrows[i].setAni(&(a_arrow[i]));

        conveyors[i].setArea(ea_gameBoard);
        conveyors[i].setType(GOT_CONVEYORS);
        conveyors[i].setAni(&(a_conveyor[i]));
        conveyors[i].startAnim();

        /* TODO: FIX
        if (render.ga.getFieldNum() == 10)
        {
            conveyors[i].startAnim();
        }
         */
    }

    // trampolines
    trampolines.setArea(ea_gameBoard);
    trampolines.setType(GOT_TRAMPOLINE);
    trampolines.setAni(&a_trampoline);
    //trampolines.startAnim();

    // warps
    warps.setArea(ea_gameBoard);
    warps.setType(GOT_WARP);
    warps.setAni(&a_warp);
    //warps.startAnim();

    // powerups
    for (Uint8 i = 0; i < CPOWERUPSR; ++i)
    {
        powerups[i].setArea(ea_gameBoard);
        powerups[i].setType(GOT_POWERUPS);
        powerups[i].setAni(&(a_power[i]));
        //powerups[i].startAnim();
    }

    powerups[PRANDOM].startAnim(); // not needed


    // fires
    for (Uint8 i = 0; i < CFIRES; ++i)
    {
        fires[i].setArea(ea_gameBoard);
        fires[i].setType(GOT_FIRES);
        fires[i].setAni(&(a_flame[i]));
        fires[i].startAnim();
    }

}

bool CResources::LoadPcx ()
{
    CPcxReader pcxreader;
    ostringstream stm;
    char buffer [MAX_BUFFER] = "";

    for (Uint16 i = 0; i < CFIELDS; ++i)
    {
        stm << PATH_DIR_RES << "FIELD" << i << ".PCX";
        snprintf(buffer, MAX_BUFFER, _("(%d/%d) DATA/RES/FIELD%d.PCX"), i, CFIELDS, i);
        m_statusString.assign(buffer);
        er.report(LOG_INFO, _("%s: Parse file '%s'.\n"), AT, stm.str().c_str());

        if (!(b_field[i] = pcxreader.read(stm.str())))
        {
            return false;
        }
        stm.str("");
    }
    return true;
}

bool CResources::loadSounds ()
{
    ostringstream stm;

    stm << PATH_DIR_SOUNDS << "click.wav";

    s_chooseSound = Mix_LoadWAV(stm.str().c_str());
    m_statusString.assign(stm.str());
    er.report(LOG_INFO, _("%s: Parse file '%s'.\n"), AT, stm.str().c_str());

    if(s_chooseSound == NULL)
    {
        er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str());
        return false;
    }

    stm.str("");
    for (Uint16 i = 0; i < C_DIE_SOUNDS; i++)
    {
        stm << PATH_DIR_SOUNDS << "die" << i << ".wav";
        s_dieSounds[i] = Mix_LoadWAV(stm.str().c_str());
        m_statusString.assign(stm.str());
        er.report(LOG_INFO, _("%s: Parse file '%s'.\n"), AT, stm.str().c_str());

        if(s_dieSounds[i] == NULL)
        {
            er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str());
            return false;
        }
        stm.str("");
    }

    stm.str("");
    for (Uint16 i = 0; i < C_EXPLODE_SOUNDS; i++)
    {
        stm << PATH_DIR_SOUNDS << "explode" << i << ".wav";
        s_explodeSounds[i] = Mix_LoadWAV(stm.str().c_str());
        m_statusString.assign(stm.str());
        er.report(LOG_INFO, _("%s: Parse file '%s'.\n"), AT, stm.str().c_str());

        if(s_explodeSounds[i] == NULL)
        {
            er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str());
            return false;
        }
        stm.str("");
    }

    return true;
}

Mix_Chunk * CResources::getChooseSound ()
{
    return s_chooseSound;
}

Mix_Chunk * CResources::getDieSound(Uint16 i)
{
    assert(i < C_DIE_SOUNDS);
    return s_dieSounds[i];
}

Mix_Chunk * CResources::getExplodeSound(Uint16 i)
{
    assert(i < C_EXPLODE_SOUNDS);
    return s_explodeSounds[i];
}

