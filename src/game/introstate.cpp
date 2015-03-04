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

#include <def/sdl.h>
#include <SDL/SDL_image.h> // need if glSDL?

#include <render/stringdraw.h>
#include <core/resources.h>

#include <game/introstate.h>
#include <game/menustate.h>

CIntroState CIntroState::m_IntroState;
extern SDL_Thread* resThread;

void CIntroState::Init ()
{
    er.setLevel(LOG_INFO);

    SDL_Surface *temp = IMG_Load(DATA_DIR "gui/loading.png");
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

    er.report(LOG_INFO, _("%s: CIntroState Init\n"), AT);
}

void CIntroState::Cleanup ()
{
    SDL_FreeSurface(bg);

    er.report(LOG_INFO, _("%s: CIntroState Cleanup\n"), AT);
}

void CIntroState::Pause ()
{
    er.report(LOG_INFO, _("%s: CIntroState Pause\n"), AT);
}

void CIntroState::Resume ()
{
    er.report(LOG_INFO, _("%s: CIntroState Resume\n"), AT);
}

void CIntroState::HandleEvents ()
{
}

void CIntroState::Update ()
{
    // load the game resources
    if (CResources::instance()->Loaded())
    {
        //int res = 0;
        //SDL_WaitThread(resThread, &res);
        //if( res )
        //{
        //    er.report(LOG_ERROR, _("%s: Thread Resources: loading resources failed.\n"), AT);
        //}
        //else
        //{
            er.report(LOG_INFO, _("%s: Thread Resources: loading resources successfully done.\n"), AT);
        //}

        CGameEngine::instance()->ChangeState(CMenuState::Instance());
    }
}

void CIntroState::Draw ()
{
    if (firstFlip)
    {
        SDL_BlitSurface(bg, NULL, SDL_GetVideoSurface(), NULL);
        CGameEngine::instance()->InvalidateArea(0, 0, 0, 0);
        firstFlip = false;
    }
    else
    {
        Uint16 w = CStringDraw::instance()->getWidth(CResources::instance()->StatusString()) + 50; // + 50 -- fix the underflow 
        Uint16 h = CFontSingleton::instance()->font.getSymbolHeight();
        Sint16 x = 10;
        Sint16 y = WIN_HEIGHT - h - 10;

        SDL_Rect r = {x, y, w, h};
        SDL_BlitSurface(bg, &r, SDL_GetVideoSurface(), &r);
        CStringDraw::instance()->drawString(x, y, CResources::instance()->StatusString());
        CGameEngine::instance()->InvalidateArea(x, y, w, h);
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
