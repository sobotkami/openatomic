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

#include "localserver.h"

#include <core/gameengine.h>
#include <core/gamestate.h>
#include <network/debugnetwork.h>

#include <game/playstate.h>
#include <game/resultsstate.h>
#include <game/menustate.h>

#include <iostream>
using namespace std;

// ugly...
extern CGO_Player players[CCOLORS];

CLocalServer::CLocalServer()
{
    er.setLevel(LOG_INFO);

    cout << "CLocalServer()" << endl;
    network = NULL;
}


void CLocalServer::player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life)
{
    players[player].move(x1, y1, x2, y2, state, life);
}

void CLocalServer::bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state)
{
    switch(state)
    {
    case bombDrop:
        CPlayState::Instance()->dropBomb(x, y, (imagecolors)player);
        break;
    case bombExplode:
        CPlayState::Instance()->explodeBomb(index);
        break;
    case bombDud:
        er.report(LOG_WARNING, _("%s: Unimplemented: %s\n"), AT, "bomb dud");
        break;
    case bombMove:
        er.report(LOG_WARNING, _("%s: Unimplemented: %s\n"), AT, "bomb move");
        break;
    default:
        er.report(LOG_WARNING, _("%s: Unknown state: %d\n"), AT, state);
    }
}

void CLocalServer::inventory(Uint8 player, powerup_t powerup, Sint8 value)
{
    players[player].setPowerup((Uint32)powerup, value);
}

void CLocalServer::updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup)
{
    cout << "updateField!" << endl;
    CGameEngine::instance()->getServer()->updateField(x, y, occupancy, powerup);
}

void CLocalServer::setDefPlPos(Uint8 player, Uint8 x, Uint8 y)
{
    cout << "sefDefPlPos!" << endl;
    CGameEngine::instance()->getServer()->setDefPlPos(player, x, y);
}


void CLocalServer::changeMenu(enum guiScreen menu)
{
    cout << "changeMenu(" << (int)menu << ")" << endl;
    CGameEngine::instance()->ChangeTopGUI(menu);
}

void CLocalServer::startGame()
{
    cout << "Start game!" << endl;
    CGameEngine::instance()->ChangeState(CPlayState::Instance());
}

void CLocalServer::showResults()
{
    cout << "Show results!" << endl;
    CGameEngine::instance()->ChangeState(CResultsState::Instance());
}

void CLocalServer::exitGame()
{
    cout << "Exit game!" << endl;
    CGameEngine::instance()->ChangeState(CMenuState::Instance());
    CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
}

void CLocalServer::registerPlayer(Uint8 player, const char *node, plmantype_t control)
{
    CGameState::instance()->registerPlayer(player, node, control);
}

void CLocalServer::unregisterPlayer(Uint8 player)
{
    CGameState::instance()->unregisterPlayer(player);
}

void CLocalServer::think()
{
    if (isNetworkRunning())
    {
        if( network->isRunning() )
        {
            msg_t msg;
            while( network->get(msg) )
            {
                // debug
                CDebugNetwork::printMessagePayload("S <--", msg);

                //.... parse message and call functions ....

                if (msg.forward)
                {
                    msg.forward = false;
                    CDebugNetwork::printMessagePayload("S -->", msg);
                    network->send(msg);
                }
            }
        }
        else
        {
            er.report(LOG_WARNING, _("%s: Network failure?\n"), AT);
        }
    }
}

void CLocalServer::startNetwork(const char * hostname)
{
    if (isNetworkRunning())
    {
        er.report(LOG_WARNING, _("%s: Network server is running. Force stop.\n"), AT);
        delete network;
    }

    network = new CNetwork<msg_t>(true, hostname);
}

void CLocalServer::stopNetwork()
{
    if(isNetworkRunning())
    {
        er.report(LOG_INFO, _("%s: Stopping network.\n"), AT);
        delete network;
        network = NULL;
    }
    else
    {
        er.report(LOG_WARNING, _("%s: Network is stopped. Do nothing.\n"), AT);
    }
}

bool CLocalServer::isNetworkRunning()
{
    return network != NULL;
}
