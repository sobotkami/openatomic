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
 * File:   gamestate.h
 * Author: minji
 *
 * Created on 16. Januar 2013, 21:37
 */

#ifndef GAMESTATE_H
#define	GAMESTATE_H

#include <string>
#include <cassert>

#include <debug/errorreporter.h>
#include <def/sdl.h>
#include <def/schemetype.h>
#include <def/players.h>
#include <def/colors.h>
#include <def/gui.h>

#include <core/gameengine.h>

#include <entities/network/serverPlayer.h>

#include <SDL/SDL_net.h>

using namespace std;

class CGameState {

public:
    static CGameState *instance() {
        if (!s_instance)
            s_instance = new CGameState;
        return s_instance;
    }

    // Game + Players
    void setSchemeFile(string file);
    string getSchemeFile();

    void setSchemeType(schemeType type);
    schemeType getSchemeType();

    void setValueToWinMatch(Uint32 value);
    Uint32 getValueToWinMatch();

    void setKillsToWin(bool kills);
    bool getKillsToWin();

    void setTeamPlay(bool team);
    bool getTeamPlay();

    void setPlayerGuiState(plmantype_t control, Uint8 player);
    playerStateGui getPlayerGuiState(Uint8 player);

    // Network
    void setIPAddress( IPaddress ip );
    IPaddress getIPAddress();

    string getNodeName();
    void setNodeName(string nodeName);

    bool getNetworkGame();
    void setNetworkGame(bool networkGame);

    // Convert
    playerStateGui str2playerStateGui(string type);
    const char *color2str(imagecolors color);

    // Store players info
    void registerPlayer(Uint8 player, const char *node, plmantype_t control);
    void unregisterPlayer(Uint8 player);
    ServerPlayer getPlayer(Uint8 player);

private:

    CGameState()
    {
        er.setLevel(LOG_INFO);
        er.report(LOG_INFO, _("%s: CGameState()\n"), AT);

        setNodeName("node");

        for (Uint8 i = 0; i < CPLAYERS; i++ )
        {
            unregisterPlayer(i);
        }

        setNetworkGame(false);
    }

    static CGameState *s_instance;

    CErrorReporter er;

    // Gui
    //CGameEngine * m_game;
    //Uint8 GuiNo;

    // Game + Players

    // scheme file
    // scheme
    // count to win
    // activated players
    // keys
    // teams?
    string schFile;
    schemeType  schType;
    Uint32 valueToWinMatch;
    bool killsToWin; // true = kills, false = score
    bool teamPlay;

    playerStateGui playerGuiStates[CPLAYERS];


    /* Store players info */
    ServerPlayer serverPlayers[CPLAYERS];

    // Network
    IPaddress ip;
    string nodeName;

    bool networkGame;

};

#endif	/* GAMESTATE_H */

