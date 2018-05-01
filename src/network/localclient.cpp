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

#include "localclient.h"

#include <entities/network/networkMessage.h>

#include <core/gameengine.h>
#include <core/gamestate.h>
#include <game/playstate.h>
#include <core/go_player.h>

CLocalClient::CLocalClient()
{
	//counter = 0;

	cout << "CLocalClient()" << endl;
}

CLocalClient::~CLocalClient()
{
	cout << "~CLocalClient()" << endl;
}

void CLocalClient::player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life)
{
	CGameEngine::instance()->getServer()->player(player, x1, y1, x2, y2, state, life);
}

void CLocalClient::bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state)
{
	CGameEngine::instance()->getServer()->bomb(player, x, y, index, state);
}

void CLocalClient::inventory(Uint8 player, powerup_t powerup, Sint8 value)
{
	CGameEngine::instance()->getServer()->inventory(player, powerup, value);
}

void CLocalClient::updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup)
{
	CGameEngine::instance()->getServer()->updateField(x, y, occupancy, powerup);
}

void CLocalClient::setDefPlPos(Uint8 player, Uint8 x, Uint8 y)
{
	CGameEngine::instance()->getServer()->setDefPlPos(player, x, y);
}

void CLocalClient::changeMenu(enum guiScreen menu)
{
	CGameEngine::instance()->getServer()->changeMenu(menu);
}

void CLocalClient::startGame()
{
	CGameEngine::instance()->getServer()->startGame();
}

void CLocalClient::showResults()
{
	CGameEngine::instance()->getServer()->showResults();
}

void CLocalClient::exitGame()
{
	CGameEngine::instance()->getServer()->exitGame();
}


void CLocalClient::registerPlayer(Uint8 player, const char *node, plmantype_t control)
{
	CGameEngine::instance()->getServer()->registerPlayer(player, node, control);
}

void CLocalClient::unregisterPlayer(Uint8 player)
{
	CGameEngine::instance()->getServer()->unregisterPlayer(player);
}

void CLocalClient::think()
{
	/*
	if ((counter % 1000 ) == 0 )
	{
		cout << "Local client thinking (" << counter << ") ..." << endl;
	}

	counter++;
	*/
}

bool CLocalClient::isConnected()
{
	return true;
}
