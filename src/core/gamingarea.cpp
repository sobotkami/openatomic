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


#include "gamingarea.h"
#include <cassert>

inline string trimString (string str)
{
    size_t start = 0;
    size_t end = str.size() - 1;
    string res = "";

    while (str[start] == ' ' || str[start] == '\t')
    {
        start++;
    }

    while (str[end] == '\n' || str[end] == '\r' || str[end] == ' ' || str[end] == '\t')
    {
        end--;
    }

    if (start > str.size()) // || end < 0 ) -- end is signed int
    {
        res = "";
        return res;
    }

    return str.substr(start, end - start + 1);
}

void CGamingArea::addArrow (Uint8 x, Uint8 y, Uint8 direction)
{
    if (x >= GA_W || y >= GA_H)
    {
        er.report(LOG_WARNING, _("%s: addArrow(%d, %d) - x or y out of range.\n"), AT, x, y);
        return;
    }

    if (((this->board[y][x]).arrow = (t_direction *) malloc(sizeof (t_direction))) == NULL)
    {
        er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, sizeof (t_direction));
        exit(1);
    }

    (this->board[y][x]).arrow->direction = direction;
}

void CGamingArea::addConveyor (Uint8 x, Uint8 y, Uint8 direction)
{
    if (x >= GA_W || y >= GA_H)
    {
        er.report(LOG_WARNING, _("%s: addConveyor(%d, %d) - x or y out of range.\n"), AT, x, y);
        return;
    }

    if (((this->board[y][x]).conveyor = (t_direction *) malloc(sizeof (t_direction))) == NULL)
    {
        er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, sizeof (t_direction));
        exit(1);
    }

    (this->board[y][x]).conveyor->direction = direction;
}

void CGamingArea::addTrampoline (Uint8 x, Uint8 y)
{
    if (x >= GA_W || y >= GA_H)
    {
        er.report(LOG_WARNING, _("%s: addTrampoline(%d, %d) - x or y out of range.\n"), AT, x, y);
        return;
    }

    if (((this->board[y][x]).trampoline = (Uint8 *) malloc(sizeof (Uint8))) == NULL)
    {
        er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, sizeof (Uint8));
        exit(1);
    }

    *((this->board[y][x]).trampoline) = 5;
}

void CGamingArea::addWarp (Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2)
{
    if (x1 >= GA_W || y1 >= GA_H || x2 >= GA_W || y2 >= GA_H)
    {
        er.report(LOG_WARNING, _("%s: addWarp(%d, %d, %d, %d) - x or y out of range.\n"), AT, x1, y1, x2, y2);
        return;
    }

    if ((this->board[y1][x1]).warp == NULL)
    {
        if (((this->board[y1][x1]).warp = (t_warp *) malloc(sizeof (t_warp))) == NULL)
        {
            er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, sizeof (t_warp));
            exit(1);
        }
    }

    if ((this->board[y2][x2]).warp == NULL)
    {
        if (((this->board[y2][x2]).warp = (t_warp *) malloc(sizeof (t_warp))) == NULL)
        {
            er.report(LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, sizeof (t_warp));
            exit(1);
        }
    }

    (this->board[y1][x1]).warp->x = x2;
    (this->board[y1][x1]).warp->y = y2;

    (this->board[y2][x2]).warp->x = -1;
    (this->board[y2][x2]).warp->y = -1;
}

CGamingArea::CGamingArea ()
{
    er.setLevel(LOG_WARNING);

    memset(&(this->board), 0, sizeof (t_field) * GA_W * GA_H);

    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            (this->board[y][x]).bomb = false;
            (this->board[y][x]).powerup = -1;
            (this->board[y][x]).fire.state = -1;

            (this->board[y][x]).trampoline = NULL;
            (this->board[y][x]).arrow = NULL;
            (this->board[y][x]).conveyor = NULL;
            (this->board[y][x]).warp = NULL;
        }
    }

    for (Uint32 i = 0; i < CPOWERUPS; i++)
    {
        //(this->schPowerups[i]).bornwith = 0;
        (this->schPowerups[i]).forbidden = 0;
        (this->schPowerups[i]).hasOverride = 0;
        (this->schPowerups[i]).overrideValue = 0;
    }

    // uuh, ugly..
    schPowerups[PBOMB ].bornwith = BWBOMB;
    schPowerups[PFLAME ].bornwith = BWFLAME;
    schPowerups[PSKATE ].bornwith = BWSKATE;
    schPowerups[PKICKER ].bornwith = BWKICKER;
    schPowerups[PJELLY ].bornwith = BWJELLY;
    schPowerups[PTRIGGER ].bornwith = BWTRIGGER;
    schPowerups[PPUNCH ].bornwith = BWPUNCH;
    schPowerups[PGRAB ].bornwith = BWGRAB;
    schPowerups[PSPOOGE ].bornwith = BWSPOOGE;
    schPowerups[PGOLDFLAME].bornwith = BWGOLDFLAME;
    schPowerups[PDISEASE ].bornwith = BWDISEASE;
    schPowerups[PDISEASE3 ].bornwith = BWDISEASE3;

    this->fieldNum = 0;
    this->numRandomPowerups = 0;

    schUseArrowsSchema = 0;
    schUseConveyorSchema = 0;
    schUseTrampolineSchema = 0;
    schUseWarpSchema = 0;

    er.report(LOG_INFO, "%s: CGamingArea()\n", AT);
}

void CGamingArea::clearBoard ()
{
    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            (this->board[y][x]).bomb = false;
            (this->board[y][x]).occupancy = TBLANK;
            (this->board[y][x]).powerup = -1;
            (this->board[y][x]).fire.state = -1;

            if ((this->board[y][x]).trampoline != NULL)
            {
                free((this->board[y][x]).trampoline);
                (this->board[y][x]).trampoline = NULL;
                //                 cout << "free(trampoline) at " << int(x) << "," << int(y) << endl ;
            }

            if ((this->board[y][x]).arrow != NULL)
            {
                free((this->board[y][x]).arrow);
                (this->board[y][x]).arrow = NULL;
                //                 cout << "free(arrow) at " << int(x) << "," << int(y) << endl ;
            }

            if ((this->board[y][x]).conveyor != NULL)
            {
                free((this->board[y][x]).conveyor);
                (this->board[y][x]).conveyor = NULL;
                //                 cout << "free(conveyor) at " << int(x) << "," << int(y) << endl ;
            }

            if ((this->board[y][x]).warp != NULL)
            {
                free((this->board[y][x]).warp);
                (this->board[y][x]).warp = NULL;
                //                 cout << "free(warp) at " << int(x) << "," << int(y) << endl ;
            }
        }
    }
}

void CGamingArea::genDefWarps ()
{
    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            if ((this->board[y][x]).warp != NULL)
            {
                free((this->board[y][x]).warp);
                (this->board[y][x]).warp = NULL;
                //                 cout << "free(warp) at " << int(x) << "," << int(y) << endl ;
            }
        }
    }

    addWarp(2, 2, 2, GA_H - 3);
    addWarp(2, GA_H - 3, GA_W - 3, GA_H - 3);
    addWarp(GA_W - 3, GA_H - 3, GA_W - 3, 2);
    addWarp(GA_W - 3, 2, 2, 2);

}

void CGamingArea::genDefConveyors ()
{
    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            if ((this->board[y][x]).conveyor != NULL)
            {
                free((this->board[y][x]).conveyor);
                (this->board[y][x]).conveyor = NULL;
                //                 cout << "free(conveyor) at " << int(x) << "," << int(y) << endl ;
            }
        }
    }

    for (Uint32 x = 2; x < GA_W - 3; x++)
    {
        addConveyor(x, 2, AEAST);
    }

    for (Uint32 y = 2; y < GA_H - 3; y++)
    {
        addConveyor(GA_W - 3, y, ASOUTH);
    }

    for (Uint32 x = 3; x < GA_W - 2; x++)
    {
        addConveyor(x, GA_H - 3, AWEST);
    }

    for (Uint32 y = 3; y < GA_H - 2; y++)
    {
        addConveyor(2, y, ANORTH);
    }

    er.report(LOG_INFO, "%s: genDefConveyors()\n", AT);
}

void CGamingArea::genDefTrampolines ()
{
    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            if ((this->board[y][x]).trampoline != NULL)
            {
                free((this->board[y][x]).trampoline);
                (this->board[y][x]).trampoline = NULL;
                //                 cout << "free(trampoline) at " << int(x) << "," << int(y) << endl ;
            }
        }
    }

    schCountOfTrampolines = 8;
    Uint8 count = schCountOfTrampolines;
    Uint8 x, y;

    do
    {
        x = rand() % GA_W;
        y = rand() % GA_H;

        if ((this->board[y][x]).trampoline != NULL)
        {
            continue;
        }

        addTrampoline(x, y);
        count--;

    }
    while (count > 0);
}

void CGamingArea::genDefArrows ()
{
    for (Uint32 y = 0; y < GA_H; ++y)
    {
        for (Uint32 x = 0; x < GA_W; ++x)
        {
            if ((this->board[y][x]).arrow != NULL)
            {
                free((this->board[y][x]).arrow);
                (this->board[y][x]).arrow = NULL;
                //                 cout << "free(arrow) at " << int(x) << "," << int(y) << endl ;
            }
        }
    }


    if (fieldNum == 2)
    { /* The Hockey Rink */
        addArrow(8, 0, AWEST);
        addArrow(2, 2, ASOUTH);
        addArrow(12, 2, AWEST);
        addArrow(0, 4, ASOUTH);
        addArrow(4, 4, AEAST);
        addArrow(10, 4, ASOUTH);

        addArrow(4, 6, ANORTH);
        addArrow(10, 6, AWEST);
        addArrow(14, 6, ANORTH);
        addArrow(2, 8, AEAST);
        addArrow(12, 8, ANORTH);
        addArrow(6, 10, AEAST);
    }
    else if (fieldNum == 3)
    { /* Ancient Egypt */
        er.report(LOG_WARNING, _("%s: Unimplemented!\n"), AT);
    }
}

CGamingArea::~CGamingArea ()
{
    clearBoard();
}

void CGamingArea::setFieldValBomb (Uint8 x, Uint8 y, bool state)
{
    assert(x < GA_W);
    assert(y < GA_H);
    board[y][x].bomb = state;
}

void CGamingArea::setFieldValFire (Uint8 x, Uint8 y, Sint8 state, imagecolors color)
{
    assert(x < GA_W);
    assert(y < GA_H);
    if (state < -2 || state > FBRICK) // state >= CFIRES)
    {
        er.report(LOG_WARNING, _("%s: setFieldValFire(%d,%d,%d,%d) -- invalid state!\n"), AT, x, y, state, (Uint32) color);
        return;
    }
    else
    {
        if (state == -1)
        {
            board[y][x].fire.ticksFireup = 0;
        }
        else
        {
            board[y][x].fire.ticksFireup = SDL_GetTicks();
        }

        board[y][x].fire.state = state;
        board[y][x].fire.color = color;

        er.report(LOG_INFO, _("%s: setFieldValFire(%d,%d,%d,%d)\n"), AT, x, y, state, (Uint32) color);
    }

}

void CGamingArea::setFieldValPowerup (Uint8 x, Uint8 y, Sint8 powerup)
{
    assert(x < GA_W);
    assert(y < GA_H);
    if (powerup < -1 || powerup > CPOWERUPSR)
    {
        er.report(LOG_WARNING, _("%s: setFieldValPowerup(%d,%d,%d) -- invalid powerup!\n"), AT, x, y, powerup);
        return;
    }
    else
    {
        if (powerup == PRANDOM)
        {
            numRandomPowerups++;
        }

        if (board[y][x].powerup == PRANDOM && powerup == -1)
        {
            numRandomPowerups--;
        }

        board[y][x].powerup = powerup;
        er.report(LOG_INFO, _("%s: [%d,%d].powerup = %d\n"), AT, x, y, powerup);
    }
}

void CGamingArea::setFieldValOccupancy (Uint8 x, Uint8 y, Sint8 occupancy)
{
    assert(x < GA_W);
    assert(y < GA_H);
    if (occupancy < -1 || occupancy >= CTILES)
    {
        er.report(LOG_WARNING, _("%s: setFieldValOccupancy(%d,%d,%d) -- invalid occupancy!\n"), AT, x, y, occupancy);
        return;
    }
    else
    {
        board[y][x].occupancy = occupancy;
        er.report(LOG_INFO, _("%s: [%d,%d].occupancy = %d\n"), AT, x, y, occupancy);
    }

}

t_powerups CGamingArea::getPowerupInfo (Uint8 powerup)
{
    assert(powerup < CPOWERUPS);
    return schPowerups[powerup];
}

Sint8 CGamingArea::getPowerupMax (Uint8 powerup)
{
    assert(powerup < CPOWERUPS);
    switch (powerup)
    {
    case PBOMB:
        return CPBOMB;
    case PFLAME:
        return CPFLAME;
    case PDISEASE:
        return CPDISEASE;
    case PKICKER:
        return CPKICKER;
    case PSKATE:
        return CPSKATE;
    case PPUNCH:
        return CPPUNCH;
    case PGRAB:
        return CPGRAB;
    case PSPOOGE:
        return CPSPOOGE;
    case PGOLDFLAME:
        return CPGOLDFLAME;
    case PTRIGGER:
        return CPTRIGGER;
    case PJELLY:
        return CPJELLY;
    case PDISEASE3:
        return CPDISEASE3;
    case PRANDOM:
        return CPRANDOM;
    default:
        return -1;
    }
}

Sint16 CGamingArea::getRandomPowerups ()
{
    return numRandomPowerups;
}

t_field CGamingArea::getBoardField (Uint8 x, Uint8 y)
{
#if 1
    if (!(x < GA_W) || !(y < GA_H))
    {
        Uint8 xOld, yOld;
        xOld = x;
        yOld = y;

        if ( !(x < GA_W) )
            x = GA_W - 1;
        if ( !(y < GA_H) )
            y = GA_H - 1;

        er.report(LOG_WARNING, "%s: Invalid getBoardField(%d,%d) --> returning(%d,%d)\n", AT, xOld, yOld, x, y);

        return board[y][x];
    }
#else
    assert(x < GA_W);
    assert(y < GA_H);
#endif

    return board[y][x];
}

Sint32 CGamingArea::getBoardFieldPath(Sint8 x, Sint8 y)
{
    t_field fi;

    if (x < 0 || y < 0 || !(x < GA_W) || !(y < GA_H) )
    {
        return 9;
    }
    else
    {
        fi = board[y][x];
        if (
                fi.bomb == true ||
                fi.fire.state != -1 ||
                fi.occupancy != TBLANK
                //fi.powerup != -1 || // fix?
                //fi.conveyor != NULL ||
                )
        {
            return 9;
        }
        else
        {
            return 1;
        }
    }
}

t_player CGamingArea::getPlayerInfo (Uint8 pno)
{
    assert(pno < CPLAYERS);
    return schPlayers[pno];
}

void CGamingArea::setDefPlPos(Uint8 player, Uint8 x, Uint8 y)
{
    assert(player < CPLAYERS);
    schPlayers[player].x = x;
    schPlayers[player].y = y;
}

Uint32 CGamingArea::getFieldNum ()
{
    return this->fieldNum;
}

void CGamingArea::setFieldNum (Uint32 num)
{
    // if ( num < 0 || num >= CFIELDS ) // num is signed int
    if (num >= CFIELDS)
    {
        this->fieldNum = 0;
    }
    else
    {
        this->fieldNum = num;
    }
}

bool CGamingArea::loadScheme (string filename)
{
    ifstream is;
    string line;

    char *pch;

    Sint8 row; //, col;
    char * columns;

    Sint8 pno;
    t_player player;
    t_powerups powerup;
    //     t_direction arrow;
    //     t_direction conveyor;
    Sint8 x1, y1, x2, y2;

    is.open(filename.c_str(), ios::binary);

    if (is.is_open())
    {
        er.report(LOG_INFO, "%s: Trying to parsing file %s.\n", AT, filename.c_str());
        
        while (is.good())
        {
            while (getline(is, line))
            {
                if (line.size() > 1)
                {
                    line = trimString(line);

                    if (line[0] != ';')
                    { /* Normalni nastaveni */
                        if (line[0] != '-')
                        {
                            er.report(LOG_WARNING, _("%s: First char is not '-'!\n"), AT);
                            return false;
                        }

                        switch (line[1])
                        {
                        case 'V':
                            schVersion = atoi(line.c_str() + 3);
                            break;
                        case 'N':
                            schName = line.c_str() + 3;
                            break;
                        case 'B':
                            schBrickDensity = atoi(line.c_str() + 3);
                            if (schBrickDensity < 0 || schBrickDensity > 100)
                            {
                                schBrickDensity = 100;
                            }
                            break;
                        case 'R':
                            if ((pch = strtok((char*) (line.c_str() + 3), " \t,")) == NULL)
                            {
                                er.report(LOG_WARNING, _("%s: Empty line!\n"), AT);
                                return false;
                            }
                            row = atoi(pch);
                            if (row < 0 || row >= GA_H)
                            { /* cislo mimo rozsah radku */
                                er.report(LOG_WARNING, _("%s: Line number out of range!\n"), AT);
                                return false;
                            }
                            columns = strtok(NULL, " \t,");
                            if (columns == NULL || strlen(columns) < GA_W)
                            { /* zadne nebo malo sloupcu */
                                er.report(LOG_WARNING, _("%s: Too few columns on the line: %d!\n"), AT, (int) row);
                                return false;
                            }

                            for (Uint8 c = 0; c < GA_W; c++)
                            {
                                switch (columns[c])
                                {
                                case '#':
                                    (this->board[row][c]).occupancy = TSOLID;
                                    break;
                                case ':':
                                    (this->board[row][c]).occupancy = TBRICK;
                                    break;
                                default:
                                case '.':
                                    (this->board[row][c]).occupancy = TBLANK;
                                    break;
                                }
                            }

                            break;
                        case 'S':
                            if ((pch = strtok((char*) (line.c_str() + 3), " \t,")) == NULL)
                            {
                                er.report(LOG_WARNING, _("%s: Empty line!\n"), AT);
                                return false;
                            }
                            pno = atoi(pch);
                            if (pno < 0 || pno >= CPLAYERS)
                            {
                                er.report(LOG_WARNING, _("%s: Players no out of range!\n"), AT);
                                return false;
                            }

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                er.report(LOG_WARNING, _("%s: Empty line!\n"), AT);
                                return false;
                            }
                            player.x = atoi(pch);
                            if (player.x < 0 || player.x >= GA_W)
                            {
                                er.report(LOG_WARNING, _("%s: Players start position x out of range!\n"), AT);
                                return false;
                            }

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                er.report(LOG_WARNING, _("%s: Empty line!\n"), AT);
                                return false;
                            }
                            player.y = atoi(pch);
                            if (player.y < 0 || player.y >= GA_H)
                            {
                                er.report(LOG_WARNING, _("%s: Players start position x out of range!\n"), AT);
                                return false;
                            }

                            if ((pch = strtok(NULL, " \t,")) != NULL)
                            {
                                player.team = atoi(pch);
                                if (player.team == 0 || player.team == 1)
                                {
                                    //er.report(LOG_WARNING, _("%s: Team must have value 0 or 1!\n"), AT);
                                    //return false;
                                    player.team = 0;
                                }
                            }
                            else
                            {
                                player.team = 0;
                            }

                            schPlayers[pno] = player;

                            er.report(LOG_INFO, _("%s: Player %d starting at %d,%d, team %s\n"), AT, (int) pno, (int) schPlayers[pno].x, (int) schPlayers[pno].y, (schPlayers[pno].team == 0 ? "red" : "white"));

                            break;
                        case 'P':
                            if ((pch = strtok((char*) (line.c_str() + 3), " \t,")) == NULL)
                            {
                                return false;
                            }
                            pno = atoi(pch);
                            if (pno < 0 || pno > CPOWERUPSR)
                            {
                                return false;
                            }

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                return false;
                            }
                            powerup.bornwith = atoi(pch);

                            if (powerup.bornwith < 0)
                                powerup.bornwith = 0;

                            switch (pno)
                            {
                            case PBOMB: /* "an extra bomb" */
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWBOMB;

                                if (powerup.bornwith > CPBOMB)
                                {
                                    er.report(LOG_WARNING, _("%s: Player born with %d (was %d, maximum %d)\n"), AT, CPBOMB, powerup.bornwith, CPBOMB);
                                    powerup.bornwith = CPBOMB;
                                }
                                break;
                            case PFLAME:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWFLAME;

                                if (powerup.bornwith > CPFLAME)
                                {
                                    powerup.bornwith = CPFLAME;
                                }
                                break;
                            case PDISEASE:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWDISEASE;

                                if (powerup.bornwith > CPDISEASE)
                                {
                                    powerup.bornwith = CPDISEASE;
                                }
                                break;
                            case PKICKER:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWKICKER;

                                if (powerup.bornwith > CPKICKER)
                                {
                                    powerup.bornwith = CPKICKER;
                                }
                                break;
                            case PSKATE:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWSKATE;

                                if (powerup.bornwith > CPSKATE)
                                {
                                    powerup.bornwith = CPSKATE;
                                }
                                break;
                            case PPUNCH:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWPUNCH;

                                if (powerup.bornwith > CPPUNCH)
                                {
                                    powerup.bornwith = CPPUNCH;
                                }
                                break;
                            case PGRAB:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWGRAB;

                                if (powerup.bornwith > CPGRAB)
                                {
                                    powerup.bornwith = CPGRAB;
                                }
                                break;
                            case PSPOOGE:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWSPOOGE;

                                if (powerup.bornwith > CPSPOOGE)
                                {
                                    powerup.bornwith = CPSPOOGE;
                                }
                                break;
                            case PGOLDFLAME:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWGOLDFLAME;

                                if (powerup.bornwith > CPGOLDFLAME)
                                {
                                    powerup.bornwith = CPGOLDFLAME;
                                }
                                break;
                            case PTRIGGER:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWTRIGGER;

                                if (powerup.bornwith > CPTRIGGER)
                                {
                                    powerup.bornwith = CPTRIGGER;
                                }
                                break;
                            case PJELLY:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWJELLY;

                                if (powerup.bornwith > CPJELLY)
                                {
                                    powerup.bornwith = CPJELLY;
                                }
                                break;
                            case PDISEASE3:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWDISEASE3;

                                if (powerup.bornwith > CPDISEASE3)
                                {
                                    powerup.bornwith = CPDISEASE3;
                                }
                                break;
                            case PRANDOM:
                                if ( powerup.bornwith == 0 )
                                    powerup.bornwith = BWRANDOM;

                                if (powerup.bornwith > CPRANDOM)
                                {
                                    powerup.bornwith = CPRANDOM;
                                }
                                break;
                            default:
                                er.report(LOG_WARNING, _("%s: Unknown powerup settings!\n"), AT);
                                break;
                            }

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                return false;
                            }
                            powerup.hasOverride = atoi(pch);

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                return false;
                            }
                            powerup.overrideValue = atoi(pch);

                            if ((pch = strtok(NULL, " \t,")) == NULL)
                            {
                                return false;
                            }
                            powerup.forbidden = atoi(pch);

                            schPowerups[pno] = powerup;

                            //                             if (debug)
                            //                             {
                            //                                 cout << "Powerup " << (int)pno << " "
                            //                                      << "bornwith: " << (int)powerup.bornwith << ", "
                            //                                      << "hasOverride: " << (int)powerup.hasOverride << ", "
                            //                                      << "overrideValue: " << (int)powerup.overrideValue << ", "
                            //                                      << "forbidden: " << (int)powerup.forbidden << endl;
                            //                             }

                            break;
                        default:
                            er.report(LOG_WARNING, _("%s: Unknown parameter: %s!\n"), AT, line.c_str());
                            break;
                        }
                    }
                    else
                    {
                        if (strncmp(line.c_str(), ";EXT", 4) == 0)
                        { /* Rozsizeni */
                            switch (line[5])
                            {
                            case 'A':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    return false;
                                }
                                row = atoi(pch);
                                if (row < 0 || row >= GA_H)
                                { /* cislo mimo rozsah radku */
                                    er.report(LOG_WARNING, _("%s: Line number out of range!\n"), AT);
                                    return false;
                                }
                                columns = strtok(NULL, " \t,");
                                if (columns == NULL || strlen(columns) < GA_W)
                                { /* zadne nebo malo sloupcu */
                                    er.report(LOG_WARNING, _("%s: Too few columns on the line: %d!\n"), AT, (int) row);
                                    return false;
                                }

                                for (Uint8 c = 0; c < GA_W; c++)
                                {
                                    switch (columns[c])
                                    {
                                    case '>':
                                        addArrow(c, row, AEAST);
                                        break;
                                    case '<':
                                        addArrow(c, row, AWEST);
                                        break;
                                    case '^':
                                        addArrow(c, row, ANORTH);
                                        break;
                                    case 'v':
                                        addArrow(c, row, ASOUTH);
                                        break;
                                    default:
                                    case '.':
                                        break;
                                    }
                                }

                                break;
                            case 'C':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    return false;
                                }
                                row = atoi(pch);
                                if (row < 0 || row >= GA_H)
                                { /* cislo mimo rozsah radku */
                                    er.report(LOG_WARNING, _("%s: Line number out of range!\n"), AT);
                                    return false;
                                }
                                columns = strtok(NULL, " \t,");
                                if (columns == NULL || strlen(columns) < GA_W)
                                { /* zadne nebo malo sloupcu */
                                    er.report(LOG_WARNING, _("%s: Too few columns on the line: %d!\n"), AT, (int) row);
                                    return false;
                                }

                                for (Uint8 c = 0; c < GA_W; c++)
                                {
                                    switch (columns[c])
                                    {
                                    case '>':
                                        addConveyor(c, row, AEAST);
                                        break;
                                    case '<':
                                        addConveyor(c, row, AWEST);
                                        break;
                                    case '^':
                                        addConveyor(c, row, ANORTH);
                                        break;
                                    case 'v':
                                        addConveyor(c, row, ASOUTH);
                                        break;
                                    default:
                                    case '.':
                                        break;
                                    }
                                }

                                break;
                            case 'T':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    return false;
                                }
                                row = atoi(pch);
                                if (row < 0 || row >= GA_H)
                                { /* cislo mimo rozsah radku */
                                    er.report(LOG_WARNING, _("%s: Line number out of range!\n"), AT);
                                    return false;
                                }
                                columns = strtok(NULL, " \t,");
                                if (columns == NULL || strlen(columns) < GA_W)
                                { /* zadne nebo malo sloupcu */
                                    er.report(LOG_WARNING, _("%s: Too few columns on the line: %d!\n"), AT, (int) row);
                                    return false;
                                }

                                for (Uint8 c = 0; c < GA_W; c++)
                                {
                                    if (columns[c] == '@')
                                    {
                                        addTrampoline(c, row);
                                    }
                                }

                                break;
                            case 'W':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    er.report(LOG_WARNING, _("%s: Missing x1!\n"), AT);
                                    return false;
                                }
                                x1 = atoi(pch);
                                if (x1 < 0 || x1 >= GA_W)
                                {
                                    er.report(LOG_WARNING, _("%s: x1 out of range!\n"), AT);
                                    return false;
                                }

                                pch = strtok(NULL, " \t,");
                                if (pch == NULL)
                                {
                                    er.report(LOG_WARNING, _("%s: Missing y1!\n"), AT);
                                    return false;
                                }
                                y1 = atoi(pch);
                                if (y1 < 0 || y1 >= GA_H)
                                {
                                    er.report(LOG_WARNING, _("%s: y1 out of range!\n"), AT);
                                    return false;
                                }

                                pch = strtok(NULL, " \t,");
                                if (pch == NULL)
                                {
                                    er.report(LOG_WARNING, _("%s: Missing x2!\n"), AT);
                                    return false;
                                }
                                x2 = atoi(pch);
                                if (x2 < 0 || x2 >= GA_W)
                                {
                                    er.report(LOG_WARNING, _("%s: x2 out of range!\n"), AT);
                                    return false;
                                }

                                pch = strtok(NULL, " \t,");
                                if (pch == NULL)
                                {
                                    er.report(LOG_WARNING, _("%s: Missing y2!\n"), AT);
                                    return false;
                                }
                                y2 = atoi(pch);
                                if (y2 < 0 || y2 >= GA_H)
                                { /* cislo mimo rozsah radku */
                                    er.report(LOG_WARNING, _("%s: y2 out of range!\n"), AT);
                                    return false;
                                }

                                addWarp(x1, y1, x2, y2);

                                break;
                            case 'U':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    return false;
                                }
                                pno = atoi(pch);

                                if ((pch = strtok(NULL, " \t,")) == NULL)
                                {
                                    return false;
                                }

                                switch (pno)
                                {
                                case 0:
                                    schUseArrowsSchema = atoi(pch);
                                    if (schUseArrowsSchema != 0 && schUseArrowsSchema != 1)
                                    {
                                        schUseArrowsSchema = 0;
                                    }
                                    break;
                                case 1:
                                    schUseConveyorSchema = atoi(pch);
                                    if (schUseConveyorSchema != 0 && schUseConveyorSchema != 1)
                                    {
                                        schUseConveyorSchema = 0;
                                    }
                                    break;
                                case 2:
                                    schUseTrampolineSchema = atoi(pch);
                                    if (schUseTrampolineSchema != 0 && schUseTrampolineSchema != 1)
                                    {
                                        schUseTrampolineSchema = 0;
                                    }
                                    break;
                                case 3:
                                    schUseWarpSchema = atoi(pch);
                                    if (schUseWarpSchema != 0 && schUseWarpSchema != 1)
                                    {
                                        schUseWarpSchema = 0;
                                    }
                                    break;
                                default:
                                    er.report(LOG_WARNING, _("%s: Unknown parameter: %s!\n"), AT, line.c_str());
                                    break;
                                }

                                break;
                            case 'V':
                                if ((pch = strtok((char*) (line.c_str() + 6), " \t,")) == NULL)
                                {
                                    return false;
                                }
                                pno = atoi(pch);

                                if ((pch = strtok(NULL, " \t,")) == NULL)
                                {
                                    return false;
                                }

                                switch (pno)
                                {
                                case 0:
                                    schCountOfTrampolines = atoi(pch);
                                    if (schCountOfTrampolines < 0 || schCountOfTrampolines >= ((GA_H - 1)*(GA_W - 1)))
                                    {
                                        schCountOfTrampolines = 8;
                                    }
                                    break;
                                case 1:
                                    schRandomExitWarp = atoi(pch);
                                    if (schRandomExitWarp != 0 && schRandomExitWarp != 1)
                                    {
                                        schRandomExitWarp = 0;
                                    }
                                    break;
                                default:
                                    er.report(LOG_WARNING, _("%s: Unknown parameter: %s!\n"), AT, line.c_str());
                                    break;
                                }

                                break;
                            default:
                                er.report(LOG_WARNING, _("%s: Unknown parameter: %s!\n"), AT, line.c_str());
                                break;
                            }
                        }
                        else
                        { /* komentare */

                        }
                    }
                }
            }
        }
        is.close();

        if (schUseWarpSchema == 0)
        {
            genDefWarps();
        }

        if (schUseConveyorSchema == 0)
        {
            genDefConveyors();
        }

        if (schUseTrampolineSchema == 0)
        {
            genDefTrampolines();
        }

        if (schUseArrowsSchema == 0)
        {
            genDefArrows();
        }

        for (Sint32 y = 0; y < GA_H; ++y)
        {
            for (Sint32 x = 0; x < GA_W; ++x)
            {
                if ((fieldNum == 4 && (this->board[y][x]).warp != NULL))
                {
                    (this->board[y][x]).occupancy = TBLANK;
                }

                for (Uint32 i = 0; i < CPLAYERS; i++)
                {
                    if (schPlayers[i].x == x && schPlayers[i].y == y)
                    {
                        (this->board[y][x]).occupancy = TBLANK;
                    }
                }
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

