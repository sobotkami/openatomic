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
 * File:   gameresults.h
 * Author: minji
 *
 * Created on 16. Januar 2013, 21:29
 */

#ifndef GAMERESULTS_H
#define	GAMERESULTS_H

#include <debug/errorreporter.h>
#include <def/sdl.h>
#include <def/players.h>
#include <def/colors.h>

typedef struct {
	Uint16 score; // 0, 1, ...
	Sint16 kills; // ..., -1, 0, 1, ...
	Uint8 team; // false (0) = red, true (1) = white team
} playerResult;

class CGameResults {
public:
	/* mainly */
	void ResetResults();
	void UpdateKills(imagecolors id, Sint8 kills);
	/* sets */
	void setWinner(Sint8 id);
	void setValueToWinMatch(Uint32 value);
	void setKillsToWin(bool value);
	void setTeamPlay(bool value);
	/* gets */
	Sint16 getMatchWinner();
	Sint16 getGameWinner();
	Uint16 getScore(Uint8 id);
	Sint16 getKills(Uint8 id);
	Uint32 getValueToWinMatch();
	bool getKillsToWin();
	bool getTeamPlay();

	static CGameResults *instance() {
		return &m_GameResults;
	}
private:
	CGameResults();
	static CGameResults m_GameResults;
	CErrorReporter er;

	playerResult results [CPLAYERS];
	Sint16 gameWinner;
	Sint16 matchWinner;
	Uint32 valueToWinMatch;
	bool killsToWin; // true = kills, false = score
	bool teamPlay;

};

#endif	/* GAMERESULTS_H */

