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
 * File:   gameresults.cpp
 * Author: minji
 * 
 * Created on 16. Januar 2013, 21:29
 */

#include "gameresults.h"

CGameResults CGameResults::m_GameResults;

CGameResults::CGameResults ()
{
	er.setLevel(LOG_INFO);

	setKillsToWin(false); // DEFAULT: kills to win 
	setValueToWinMatch(3); // DEFAULT: 10 KILLS = WIN MATCH
	setTeamPlay(false);

	ResetResults();
}

void CGameResults::ResetResults ()
{
	for (Uint32 i = 0; i < CPLAYERS; i++)
	{
		results[i].score = 0;
		results[i].kills = 0;
	}

	matchWinner = -1;
	gameWinner = -1;
}

Uint16 CGameResults::getScore (Uint8 id)
{
	if (id < CPLAYERS)
		return results[id].score;
	else
		return 0;
}

Sint16 CGameResults::getKills (Uint8 id)
{
	if (id < CPLAYERS)
		return results[id].kills;
	else
		return 0;
}

void CGameResults::UpdateKills (imagecolors id, Sint8 kills)
{
	if (id < CPLAYERS)
		results[id].kills += kills;

}

Sint16 CGameResults::getMatchWinner ()
{
	return matchWinner;
}

Sint16 CGameResults::getGameWinner ()
{
	return gameWinner;
}

void CGameResults::setWinner (Sint8 id)
{
	if (id < CPLAYERS)
	{
		results[id].score++;
		gameWinner = id;

		if (teamPlay)
		{
			Uint32 maxRed = 0;
			Uint32 maxWhite = 0;
			for (Uint32 i = 0; i < CPLAYERS; i++)
			{
				if (results[i].team == TEAM_RED)
				{
					maxRed += killsToWin ? results[i].kills : results[i].score;
				}
				else
				{
					maxWhite += killsToWin ? results[i].kills : results[i].score;
				}
			}
			if (maxRed >= valueToWinMatch)
			{
				matchWinner = TEAM_RED;
			}
			else if (maxWhite >= valueToWinMatch)
			{
				matchWinner = TEAM_WHITE;
			}
			else
			{
				matchWinner = -1;
			}
		}
		else
		{
			Uint32 max = killsToWin ? results[0].kills : results[0].score;
			Uint32 idmax = 0;
			for (Uint32 i = 1; i < CPLAYERS; i++)
			{
				if (killsToWin ? results[i].kills : results[i].score > max)
				{
					max = killsToWin ? results[i].kills : results[i].score;
					idmax = i;
				}
			}
			if (max >= valueToWinMatch)
			{
				matchWinner = idmax;
			}
			else
			{
				matchWinner = -1;
			}
		}
	}
	else
	{
		gameWinner = -1;
	}
}

void CGameResults::setValueToWinMatch (Uint32 value)
{
	valueToWinMatch = value;
}

Uint32 CGameResults::getValueToWinMatch ()
{
	return valueToWinMatch;
}

void CGameResults::setKillsToWin (bool value)
{
	killsToWin = value;
}

bool CGameResults::getKillsToWin ()
{
	return killsToWin;
}

void CGameResults::setTeamPlay (bool value)
{
	teamPlay = value;
}

bool CGameResults::getTeamPlay ()
{
	return teamPlay;
}
