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

#ifndef AIMODEL_H
#define AIMODEL_H

#include "../def/colors.h"
#include "../def/players.h"
#include "../core/gamingarea.h"
#include "aiplayer.h"
//#include "aigamearea.h"
#include "../def/sdl.h"
#include "../def/point.h"
#include "../3rdparty/astar/mapsearchnode.h"
#include <debug/errorreporter.h>

#include "../core/concurentqueue.h"
#include "../core/message.h"

class CAIModel
{
public:
	CAIModel(CGamingArea * ga);
	CAIModel();

	void addPlayer(imagecolors color, Uint8 x, Uint8 y);
	void addPlayer(imagecolors color);
	void removePlayer(imagecolors color);

	void setGa(CGamingArea *ga);

	void doMove(CPriorityConcurrentQueue<CMessage> *q );

private:
	CErrorReporter er;
	CGamingArea *ga;
	CAIPlayer aip[CPLAYERS];
	AStarSearch<MapSearchNode, CGamingArea> astarsearch;
};

#endif // AIMODEL_H
