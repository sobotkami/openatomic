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

#ifndef AIGAMEAREA_H
#define AIGAMEAREA_H

#include "../def/sdl.h"
#include "aigameareafield.h"
#include "../global.h"
#include "../core/gamingarea.h"
#include <cassert>

class CAIGameArea
{
private:
	CAIGameAreaField aiga [GA_H][GA_W];


public:
	CAIGameArea();

	CAIGameAreaField get(Uint8 x, Uint8 y);

	void sync( CGamingArea * ga );
};

#endif // AIGAMEAREA_H
