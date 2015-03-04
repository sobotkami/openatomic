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
 * File:   GO_Bomb.cpp
 * Author: minji
 * 
 * Created on 4. Januar 2013, 13:32
 */

#include "go_bomb.h"

extern CAnimation a_bomb[4];

CGO_Bomb::CGO_Bomb ()
{
    ani = &(a_bomb[BREG]);
    setArea(ea_gameBoard);
    ticksTimeoutDefault = 2000;
    ticksTimeout = ticksTimeoutDefault;

    moveAbs(0, 0);
}

bool CGO_Bomb::getActive ()
{
    return active;
}

void CGO_Bomb::setActive (bool active)
{
    if (active)
    {
        ticksActivated = SDL_GetTicks();
    }
    else
    {
        ticksActivated = 0;
    }
    this->active = active;
}

void CGO_Bomb::setFlameLength (Uint8 len)
{
    this->flameLength = len;
}

Uint8 CGO_Bomb::getFlameLength ()
{
    return flameLength;
}

Uint32 CGO_Bomb::ticksToExplode ()
{
    Sint32 delta = -(SDL_GetTicks() - (ticksActivated + ticksTimeout));
    return delta < 0 ? 0 : delta;
}

void CGO_Bomb::setTimeout (Uint32 timeout)
{
    ticksTimeout = timeout;
}
