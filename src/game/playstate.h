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

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "../def/sdl.h"
#include "../def/colors.h"
#include "programstate.h"
#include <debug/errorreporter.h>
#include "../core/gamingarea.h"
#include "../core/message.h"

// AI
#include "../ai/aimodel.h"

class CPlayState : public CProgramState {
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

	static CPlayState* Instance() {
		return &m_PlayState;
	}

private:
	static CPlayState m_PlayState;

	CPlayState() { }
	CErrorReporter er;

	SDL_Surface* bg;

	bool redraw;
	Uint32 r, nr;
	bool drawDebug;
	bool sdlDelay;

	// count FPS
	Uint32 cycleCount;
	Uint32 frameCount;
	double calculateCPS();
	double calculateFPS();

	void DrawBackground();
	void DrawSolidObjects();
	void DrawNonSolidObjects();
	void DrawBombsFlames();
	void DrawPlayers();
	void DrawDebug();

	void printMessagePayload( CMessage &m );

	CGamingArea * ga;

	bool roundHasWinner; // someone wins
	Uint32 ticksRoundHasWinner;
	Uint32 ticksStartGame;

	// save keystates
	Uint8* keystates;
	// PlayerX droped bomb
	//bool dropBomb[CPLAYERS];

	void playersDie(Sint16 x, Sint16 y, imagecolors color);
	void playerDie (imagecolors victim, imagecolors killer);
	//void explodeBomb(Uint16 i);
	void explodeBomb(Uint8 x, Uint8 y);
	//void dropBomb(Sint16 x, Sint16 y, imagecolors color);
	bool explodeField(Uint8 x, Uint8 y);

	void activatePlayer(Uint8 x, Uint8 y, imagecolors id);
	void movePlayer (imagecolors id, bool west, bool east, bool north, bool south);

	// AI
	CAIModel * aim;

	// players
	imagecolors playerKey0;
	bool playerKey0Enabled;
	imagecolors playerKey1;
	bool playerKey1Enabled;

	bool playerAIEnabled;

	/* statistic */
	Uint32 updatedField;
	Uint32 updatedPlDefPos;

public:
	void dropBomb(Sint16 x, Sint16 y, imagecolors color);
	void explodeBomb(Uint16 i);

	void updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup);
	void setPlDefPos(Uint8 player, Uint8 x, Uint8 y);

};

#endif
