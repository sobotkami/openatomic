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
 * File:   gamestate.cpp
 * Author: minji
 *
 * Created on 16. Januar 2013, 21:37
 */

#include <core/gamestate.h>

#include <entities/network/networkMessage.h>

#include <sstream>
using namespace std;

CGameState * CGameState::s_instance;

void CGameState::setSchemeFile(string file)
{
	schFile = file;
}

string CGameState::getSchemeFile()
{
	return schFile;
}

void CGameState::setSchemeType(schemeType type)
{
	schType = type;
}

schemeType CGameState::getSchemeType()
{
	return schType;
}

void CGameState::setValueToWinMatch(Uint32 value)
{
	valueToWinMatch = value;
}

Uint32 CGameState::getValueToWinMatch()
{
	return valueToWinMatch;
}

void CGameState::setKillsToWin(bool kills)
{
	killsToWin = kills;
}

bool CGameState::getKillsToWin()
{
	return killsToWin;
}

void CGameState::setTeamPlay(bool team)
{
	teamPlay = team;
}

bool CGameState::getTeamPlay()
{
	return teamPlay;
}

void CGameState::setPlayerGuiState(plmantype_t control, Uint8 player)
{
	assert( player < CPLAYERS );

	er.report(LOG_INFO, "%s: setPlayerGuiState(%d, %d)\n", AT, control, player);

	//CGameEngine::instance()->getClient()->registerPlayer(player, state);
	CGameEngine::instance()->getClient()->registerPlayer(player, CGameState::instance()->getNodeName().c_str(), control);

	/*
	plmantype_t t;

	if ( state == off )
	{
		t = typeNone;
		//CGameEngine::instance()->getNetClient()->unregisterPlayer(player);
		CGameEngine::instance()->getNetClient()->registerPlayer(player, t);
	}
	else if ( state == key0 )	bool getNetworkGame();
	void setNetworkGame(bool networkGame);
	{
		t = typeKey0;
		//CGameEngine::instance()->getNetClient()->unregisterPlayer(player);
		CGameEngine::instance()->getNetClient()->registerPlayer(player, t);
	}
	else if ( state == key1 )
	{
		t = typeKey1;
		//CGameEngine::instance()->getNetClient()->unregisterPlayer(player);
		CGameEngine::instance()->getNetClient()->registerPlayer(player, t);
	}
	else if ( state == ai )
	{
		t = typeAi;
		//CGameEngine::instance()->getNetClient()->unregisterPlayer(player);
		CGameEngine::instance()->getNetClient()->registerPlayer(player, t);
	}
	*/

	//playerGuiStates[player] = state;
}

playerStateGui CGameState::getPlayerGuiState(Uint8 player)
{
	assert( player < CPLAYERS );
	return playerGuiStates[player];
}


void CGameState::setIPAddress( IPaddress ip )
{
	this->ip = ip;
}

IPaddress CGameState::getIPAddress()
{
	return ip;
}

bool CGameState::getNetworkGame()
{
	return networkGame;
}

void CGameState::setNetworkGame(bool networkGame)
{
	this->networkGame = networkGame;
}

void CGameState::setNodeName( string nodeName )
{
	// create appendix to prevent same nodename
	stringstream ss;
	ss << &nodeName;

	this->nodeName = nodeName + "@" + ss.str();
}

string CGameState::getNodeName()
{
	return nodeName;
}


void CGameState::registerPlayer(Uint8 player, const char *node, plmantype_t control)
{
	assert( player < CPLAYERS );
	serverPlayers[player].control = control;
	serverPlayers[player].node = node;
}

void CGameState::unregisterPlayer(Uint8 player)
{
	assert( player < CPLAYERS );
	serverPlayers[player].control = typeNone;
	serverPlayers[player].node = "";
}

ServerPlayer CGameState::getPlayer(Uint8 player)
{
	assert( player < CPLAYERS );
	return serverPlayers[player];
}


playerStateGui CGameState::str2playerStateGui(string type)
{
	er.report(LOG_INFO, _("%s: str2playerStateGui(%s)\n"), AT, type.c_str());

	if(type == "OFF" || type == "Off" || type == "off")
	{
		return off;
	}

	if(type == "KEY 0" || type == "Key 0" || type == "key 0" || type == "key0" )
	{
		return key0;
	}

	if(type == "KEY 1" || type == "Key 1" || type == "key 1" || type == "key1" )
	{
		return key1;
	}

	if(type == "AI" || type == "Ai" || type == "ai")
	{
		return ai;
	}

	if(type == "NET" || type == "Net" || type == "net")
	{
		return net;
	}

	return off;
}

const char * CGameState::color2str(imagecolors color)
{
	switch (color)
	{
	case cgreen:
		return _("Green");
		break;
	case cblue:
		return _("Blue");
		break;
	case cred:
		return _("Red");
		break;
	case ccyan:
		return _("Cyan");
		break;
	case cmagenta:
		return _("Magenta");
		break;
	case cyellow:
		return _("Yellow");
		break;
	case corange:
		return _("Orange");
		break;
	case cpurple:
		return _("Purple");
		break;
	case cblack:
		return _("Black");
		break;
	case cwhite:
		return _("White");
		break;
	case cbrown:
		return _("Brown");
		break;
	default:
		return _("Unknown color");
	}
}


