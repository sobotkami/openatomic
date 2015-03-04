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

#ifndef GLOBAL_H
#define GLOBAL_H

/* --------------- platform defines --------------- */

#include "def/platform.h"


//Headers
#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
    #include <pthread.h>
#elif defined(CONF_PF_WINDOWS)
//     #define _WIN32_WINNT 0x0400
    #include <windows.h>
    #include <process.h>
#endif

//Data types
#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
    #define MUTEX pthread_mutex_t
#elif defined(CONF_PF_WINDOWS)
    #define MUTEX HANDLE
#endif

/* -------------- //platform defines -------------- */


/* sizeof gameboard in boxes */
#define GA_W 15
#define GA_H 11

#define BOX_W 40
#define BOX_H 36
#define BOX_W2 20
#define BOX_H2 18

#define GOT_PLAYER      0
#define GOT_BOMB        1
#define GOT_BRICK       2
#define GOT_ARROW       3
#define GOT_WARP        4
#define GOT_TRAMPOLINE  5
#define GOT_CONVEYORS   6
#define GOT_POWERUPS    7
#define GOT_FIRES       8
#define GOT_FIRESB      9

// states of life
#define LALIVE   0
#define LDIES    1
#define LDEAD    2

// states of player
#define XSTAND   0
#define XWALK    1
#define XBLOCKED 2

// states of anis
#define SSTAND      0
#define SWALK       1
#define SCORNERHEAD 2
#define SDIE        3
#define SPUNCH      4
#define SKICK       5
#define SPICKUP     6
#define SSPIN       7

#define AWEST  0
#define AEAST  1
#define ANORTH 2
#define ASOUTH 3
#define ANONE  4


// count of..
#define CCOLORS      10
#define CDIES        24
#define CCORNERHEAD  13
#define CFIELDS      11
#define CBACKGROUNDS 7
#define CDIRECTIONS  4
#define CPOWERUPS    12
#define CPOWERUPSR   13
#define CTILES       3
#define CFIRES       9

#define CBOMBS       100

#define CGUISTATES   6

#define AWEST  0
#define AEAST  1
#define ANORTH 2
#define ASOUTH 3

#define FCENTER   0
#define FMIDWEST  1
#define FTIPWEST  2
#define FMIDEAST  3
#define FTIPEAST  4
#define FMIDNORTH 5
#define FTIPNORTH 6
#define FMIDSOUTH 7
#define FTIPSOUTH 8
#define FBRICK    9

#define CPBOMB      8
#define CPFLAME     8
#define CPDISEASE   0
#define CPKICKER    1
#define CPSKATE     4
#define CPPUNCH     1
#define CPGRAB      1
#define CPSPOOGE    1
#define CPGOLDFLAME 1
#define CPTRIGGER   1
#define CPJELLY     1
#define CPDISEASE3  0
#define CPRANDOM    0

#define BWBOMB      1
#define BWFLAME     2
#define BWDISEASE   0
#define BWKICKER    0
#define BWSKATE     0
#define BWPUNCH     0
#define BWGRAB      0
#define BWSPOOGE    0
#define BWGOLDFLAME 0
#define BWTRIGGER   0
#define BWJELLY     0
#define BWDISEASE3  0
#define BWRANDOM    0

#define PBOMB      0
#define PFLAME     1
#define PDISEASE   2
#define PKICKER    3
#define PSKATE     4
#define PPUNCH     5
#define PGRAB      6
#define PSPOOGE    7
#define PGOLDFLAME 8
#define PTRIGGER   9
#define PJELLY     10
#define PDISEASE3  11
#define PRANDOM    12

#define BDUD 0
#define BREG 1
#define BJEL 2
#define BTRI 3

#define TBLANK 0
#define TBRICK 1
#define TSOLID 2

// key repeat in ms
#define KEYREPEAT 20

#endif // GLOBAL_H
