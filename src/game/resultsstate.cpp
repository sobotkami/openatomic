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

#include "../def/sdl.h"
#include "SDL_image.h" // need if glSDL?
#include "../core/gameengine.h"
#include "resultsstate.h"
#include "playstate.h"
#include "menustate.h"
#include "../core/gameresults.h"
#include "../render/stringdraw.h"
#include "../def/colors.h"

CResultsState CResultsState::m_ResultsState;

void CResultsState::Init ()
{
	er.setLevel(LOG_INFO);

	SDL_Surface *temp = IMG_Load(PATH_DIR_RES "RESULTS.PCX");
	if (!temp)
	{
		/* image failed to load */
		er.report(LOG_ERROR, _("%s: IMG_Load: %s\n"), AT, IMG_GetError());
		SDL_Quit();
		return;
	}

	bg = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	firstFlip = true;
	showMatchWinner = false;

	er.report(LOG_INFO, _("%s: CResultsState Init\n"), AT);
}

void CResultsState::Cleanup ()
{
	SDL_FreeSurface(bg);

	er.report(LOG_INFO, _("%s: CResultsState Cleanup\n"), AT);
}

void CResultsState::Pause ()
{
	er.report(LOG_INFO, _("%s: CResultsState Pause\n"), AT);
}

void CResultsState::Resume ()
{
	er.report(LOG_INFO, _("%s: CResultsState Resume\n"), AT);
}

void CResultsState::HandleEvents ()
{
	SDL_Event event;

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			CGameEngine::instance()->Quit();
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				/* stop game? */

				/* go to menu */
				CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
				CGameEngine::instance()->ChangeState(CMenuState::Instance());
				break;
			case SDLK_RETURN:
				if (CGameResults::instance()->getMatchWinner() != -1)
				{
					// game ended, back to the menu
					CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
					CGameEngine::instance()->ChangeState(CMenuState::Instance());
				}
				else
				{
					// back to the game
					CGameEngine::instance()->ChangeState(CPlayState::Instance());
				}

				break;
			default:
				break;
			}
			break;
		default: break;
		}
	}
}

void CResultsState::Update ()
{

}

void CResultsState::Draw ()
{
	if (firstFlip)
	{
		SDL_BlitSurface(bg, NULL, SDL_GetVideoSurface(), NULL);

		// Match winner
		drawMatchWinner();
		// Game winner
		drawGameWinner();
		// Player results
		for (Uint8 i = 0; i < CPLAYERS; i++)
		{
			drawPlayerResult((imagecolors)i);
		}

		CGameEngine::instance()->InvalidateArea(0, 0, 0, 0);
		firstFlip = false;
	}
	else
	{

	}

	// Update the screen
	if ((SDL_GetVideoSurface()->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF)
	{
		SDL_Flip(SDL_GetVideoSurface());
		CGameEngine::instance()->getUglyRects().clear();
	}
	else
	{

		vector<SDL_Rect>::size_type sz = CGameEngine::instance()->getUglyRects().size();
		SDL_Rect r;

		for (Uint32 i = 0; i < sz; i++)
		{
			r = CGameEngine::instance()->getUglyRects().back();
			SDL_UpdateRect(SDL_GetVideoSurface(), r.x, r.y, r.w, r.h);
			CGameEngine::instance()->getUglyRects().pop_back();
		}
	}

	SDL_Delay(1);
}

void CResultsState::drawPlayerResult (imagecolors id)
{
	const int MAX_LEN = 300;
	char buffer [MAX_LEN] = "";
	snprintf(buffer, MAX_LEN, _("Player %d score: %d (kills: %d)"),
			 id + 1, CGameResults::instance()->getScore(id),
			 CGameResults::instance()->getKills(id));


	//Uint16 w = CStringDraw::instance()->getWidth(res);
	Uint16 h = CFontSingleton::instance()->font.getSymbolHeight();
	Sint16 x = 150;
	Sint16 y = 210 + id * (h + 5);

	CStringDraw::instance()->drawString(x, y, buffer, id);
}

void CResultsState::drawGameWinner ()
{
	const int MAX_LEN = 300;
	char buffer [MAX_LEN] = "";

	if (CGameResults::instance()->getGameWinner() != -1)
	{
		snprintf(buffer, MAX_LEN, _("Game winner was player %d!"), CGameResults::instance()->getGameWinner() + 1);
		CStringDraw::instance()->drawString(150, 140, buffer, cwhite);
	}
	else
	{
		snprintf(buffer, MAX_LEN, _("No game winner."));
		CStringDraw::instance()->drawString(150, 140, buffer, cwhite);
	}
}

void CResultsState::drawMatchWinner ()
{
	const int MAX_LEN = 300;
	char buffer [MAX_LEN] = "";

	if (CGameResults::instance()->getMatchWinner() != -1)
	{
		if (CGameResults::instance()->getTeamPlay())
		{
			snprintf(buffer, MAX_LEN, _("TEAM %d WINS THE MATCH!"), CGameResults::instance()->getMatchWinner() + 1);
		}
		else
		{
			snprintf(buffer, MAX_LEN, _("PLAYER %d WINS THE MATCH!"), CGameResults::instance()->getMatchWinner() + 1);
		}
		CStringDraw::instance()->drawString(150, 95, buffer, ccyan);
	}
	else
	{
		if (CGameResults::instance()->getKillsToWin())
		{
			snprintf(buffer, MAX_LEN, _("(Match winner mush kills %d players)"), CGameResults::instance()->getValueToWinMatch());
		}
		else
		{
			snprintf(buffer, MAX_LEN, _("(Match winner must score %d victories)"), CGameResults::instance()->getValueToWinMatch());
		}

		CStringDraw::instance()->drawString(150, 95, buffer, cwhite);
	}

}

