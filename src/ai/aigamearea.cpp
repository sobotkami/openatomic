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

#include "aigamearea.h"

CAIGameArea::CAIGameArea()
{
}

CAIGameAreaField CAIGameArea::get(Uint8 x, Uint8 y)
{
    assert(x < GA_W);
    assert(y < GA_H);
    return (aiga[y][x]);
}

void CAIGameArea::sync(CGamingArea *ga)
{
    assert(ga != NULL);

    t_field fi;

    for (Uint8 y = 0; y < GA_H; ++y)
    {
        for (Uint8 x = 0; x < GA_W; ++x)
        {
            fi = ga->getBoardField(x, y);
            if (fi.bomb == true ||
                    fi.fire.state != -1 ||
                    fi.occupancy != TBLANK
                    //fi.powerup != -1 || // fix?
                    //fi.conveyor != NULL ||
                    )
            {
                aiga[y][x].setRating(-1);
            }
            else
            {
                aiga[y][x].setRating(0);
            }
        }
    }
}

