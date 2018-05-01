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

#include <network/networkclient.h>
#include <core/gameengine.h>
#include <core/gamestate.h>
#include <network/debugnetwork.h>
#include <game/playstate.h>
#include <game/resultsstate.h>
#include <game/menustate.h>


void CNetworkClient::player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typePlayer;
    msg.payload.mPlayer.player = player;
    msg.payload.mPlayer.x1 = x1;
    msg.payload.mPlayer.y1 = y1;
    msg.payload.mPlayer.x2 = x2;
    msg.payload.mPlayer.y2 = y2;
    msg.payload.mPlayer.state = state;
    msg.payload.mPlayer.life = life;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeBomb;
    msg.payload.mBomb.player = player;
    msg.payload.mBomb.x = x;
    msg.payload.mBomb.y = y;
    msg.payload.mBomb.index = index;
    msg.payload.mBomb.state = state;


    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::inventory(Uint8 player, powerup_t powerup, Sint8 value)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeInventory;
    msg.payload.mInventory.player = player;
    msg.payload.mInventory.powerup = powerup;
    msg.payload.mInventory.value = value;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeField;
    msg.payload.mField.x = x;
    msg.payload.mField.y = y;
    msg.payload.mField.occupancy = occupancy;
    msg.payload.mField.powerup = powerup;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::setDefPlPos(Uint8 player, Uint8 x, Uint8 y)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typePlDefPos;
    msg.payload.mPlDefPos.player = player;
    msg.payload.mPlDefPos.x = x;
    msg.payload.mPlDefPos.y = y;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::changeMenu(enum guiScreen menu)
{
    cout << "[STUB] changeMenu(" << (int)menu << ")" << endl;
}

void CNetworkClient::startGame()
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeGamestate;
    //strncpy(msg.payload.mPlMan.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);
    msg.payload.mGamestate.gamestate = gameStart;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::showResults()
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeGamestate;
    //strncpy(msg.payload.mPlman.node, CGameState::instance()->getNodeName().c_str(), MSG_PLNAM_MAXLEN_NODE);
    msg.payload.mGamestate.gamestate = gameResults;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::exitGame()
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typeGamestate;
    //strncpy(msg.payload.mPlman.node, CGameState::instance()->getNodeName().c_str(), MSG_PLNAM_MAXLEN_NODE);
    msg.payload.mGamestate.gamestate = gameExit;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::registerPlayer(Uint8 player, const char *node, plmantype_t control)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    // send node
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typePlMan;
    // who play
    strncpy(msg.payload.mPlMan.node, node, MSG_NODE_MAXLEN);
    msg.payload.mPlMan.player = player;
    msg.payload.mPlMan.type = control;
    msg.payload.mPlMan.state = nodeSetup;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::unregisterPlayer(Uint8 player)
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    msg.timestamp = SDL_GetTicks();
    msg.forward = true;
    strncpy(msg.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);

    msg.type = typePlMan;
    strncpy(msg.payload.mPlMan.node, CGameState::instance()->getNodeName().c_str(), MSG_NODE_MAXLEN);
    msg.payload.mPlMan.player = player;
    msg.payload.mPlMan.type = typeNone;
    msg.payload.mPlMan.state = nodeSetup;

    // debug
    CDebugNetwork::printMessagePayload("C -->", msg);
    send(msg);
}

void CNetworkClient::think()
{
    msg_t msg;
    memset(&msg, 0, sizeof(msg_t));

    if( isRunning() )
    {
        while( get(msg) )
        {
            CDebugNetwork::printMessagePayload("C <--", msg);
            switch(msg.type)
            {
            case typeBomb:
                parseTypeBomb(msg);
                break;
            case typePlayer:
                parseTypePlayer(msg);
                break;
            case typeGamestate:
                parseTypeGameState(msg);
                break;
            case typeText:
                parseTypeText(msg);
                break;
            case typeHello:
                parseTypeHello(msg);
                break;
            case typePlMan:
                parseTypePlMan(msg);
                break;
            case typeField:
                parseTypeField(msg);
                break;
            case typePlDefPos:
                parseTypePlDefPos(msg);
                break;
            case typeInventory:
                parseTypeInventory(msg);
                break;
            default:
                er.report(LOG_WARNING, _("%s: Unsupported message type: %d\n"), AT, msg.type);
            }
        }
    }
    else
    {
        er.report(LOG_WARNING, _("%s: Network failure?\n"), AT);
        throw int(1); // network failure
        // go to main menu
    }
}

bool CNetworkClient::isConnected()
{
    return isRunning();
}

void CNetworkClient::parseTypeBomb(msg_t &msg)
{
    msg.forward = false;

    CGameEngine::instance()->getServer()->bomb(
                msg.payload.mBomb.player,
                msg.payload.mBomb.x,
                msg.payload.mBomb.y,
                msg.payload.mBomb.index,
                msg.payload.mBomb.state
                );
}

void CNetworkClient::parseTypePlayer(msg_t &msg)
{
    msg.forward = false;

    CGameEngine::instance()->getServer()->player(
                msg.payload.mPlayer.player,
                msg.payload.mPlayer.x1,
                msg.payload.mPlayer.y1,
                msg.payload.mPlayer.x2,
                msg.payload.mPlayer.y2,
                msg.payload.mPlayer.state,
                msg.payload.mPlayer.life
                );
}

void CNetworkClient::parseTypeInventory(msg_t &msg)
{
    msg.forward = false;

    CGameEngine::instance()->getServer()->inventory(
                msg.payload.mInventory.player,
                msg.payload.mInventory.powerup,
                msg.payload.mInventory.value
                );
}

void CNetworkClient::parseTypeGameState(msg_t &msg)
{
    // dont forward
    msg.forward = false;

    switch(msg.payload.mGamestate.gamestate)
    {
    case gameStart:
        CGameEngine::instance()->getServer()->startGame();
        break;
    case gameResults:
        CGameEngine::instance()->getServer()->showResults();
        break;
    case gameExit:
        CGameEngine::instance()->getServer()->exitGame();
        break;
    default:
        er.report(LOG_WARNING, _("%s: Unsupported game state: %d\n"), AT, msg.payload.mGamestate.gamestate);
    }
}

void CNetworkClient::parseTypeText(msg_t &msg)
{
    cout << "[STUB] parseTypeText()" << endl;
}

void CNetworkClient::parseTypeHello(msg_t &msg)
{
    cout << "[STUB] parseTypeHello()" << endl;
}

void CNetworkClient::parseTypePlMan(msg_t &msg)
{
    // dont forward
    msg.forward = false;

    if ( msg.payload.mPlMan.state == nodeSetup )
    {
        if (msg.payload.mPlMan.type == typeNone)
        { // unregister
            CGameEngine::instance()->getServer()->unregisterPlayer(
                        msg.payload.mPlMan.player
                        );
        }
        else
        { // register
            CGameEngine::instance()->getServer()->registerPlayer(
                        msg.payload.mPlMan.player,
                        msg.payload.mPlMan.node,
                        msg.payload.mPlMan.type
                        );
        }
    }
}

void CNetworkClient::parseTypeField(msg_t &msg)
{
    // dont forward
    msg.forward = false;

    CPlayState::Instance()->updateField(
                msg.payload.mField.x,
                msg.payload.mField.y,
                msg.payload.mField.occupancy,
                msg.payload.mField.powerup
                );
}

void CNetworkClient::parseTypePlDefPos(msg_t &msg)
{
    // dont forward
    msg.forward = false;

    CPlayState::Instance()->setPlDefPos(
                msg.payload.mPlDefPos.player,
                msg.payload.mPlDefPos.x,
                msg.payload.mPlDefPos.y
                );
}

