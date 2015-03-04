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

#include "aicontroller.h"
#include "../def/colors.h"

CAIController::CAIController(CGamingArea * ga, CGO_Player * players[])
{
    //model.setGa(ga);

    for(Uint8 i = 0; i < CPLAYERS; i++)
    {
        if(players[i]->getActive())
        {
            //model.addPlayer((imagecolors)i, players[i]->getArrayX(), players[i]->getArrayY() );
        }
    }
}

void CAIController::input()
{

}

void CAIController::update()
{
    //model.doMove();
}
