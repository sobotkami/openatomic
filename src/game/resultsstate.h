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

#ifndef RESULTSSTATE_H
#define RESULTSSTATE_H

#include "../def/sdl.h"
#include "../def/colors.h"
#include "programstate.h"
#include <debug/errorreporter.h>

class CResultsState : public CProgramState {
public:
    void Init();
    void Cleanup();

    void Pause();
    void Resume();

    void HandleEvents();
    void Update();
    void Draw();

    static CResultsState* Instance() {
        return &m_ResultsState;
    }

protected:

    CResultsState() {
    }

private:
    static CResultsState m_ResultsState;
    CErrorReporter er;

    SDL_Surface* bg;

    bool firstFlip;
    bool showMatchWinner;

    void drawPlayerResult(imagecolors id);
    void drawGameWinner();
    void drawMatchWinner();
};

#endif
