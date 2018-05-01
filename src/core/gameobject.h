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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "animation.h"
#include "../readers/pcxreader.h"
#include "../global.h"
#include <debug/errorreporter.h>
#include "../readers/anireader.h"
#include "../render/render.h"

#include "gamingarea.h"
#include <ctime>
#include <string>
#include <iostream>

using namespace std;

typedef enum {
	ea_menuMain,
	ea_menuNewGame,
	ea_menuOptions,
	ea_gamePlayers,
	ea_gameTime,
	ea_gameBoard
} earea;

/*
typedef struct {
	Uint8 delay;
	Uint16 fanim;
	CAnimation *ani;
} TimerParam;
 */
class CGameObject {
private:
	Uint8 type;
	imagecolors color;
	earea tarea; /* area on screen */
	SDL_Rect area;
	Sint16 x, y; /* position of object */

	Uint32 ticksStartAnim;
	Uint32 ticksDoneAnim;

protected:
	CAnimation * ani;
	CGamingArea * ga;
	bool animLoop;
	bool animDone;
public:
	bool getAnimDone();
	void setAnimLoop(bool loop);
	Uint32 getTicksDoneAnim();

	void setGamingArea(CGamingArea * ga);

	void setColor(imagecolors color);
	imagecolors getColor();

	void setType(Uint8 type);

	void setArea(earea area);
	SDL_Rect getArea();

	void setAni(CAnimation * ani);

	Sint8 moveRel(Sint8 x, Sint8 y);
	void moveAbs(Sint16 x, Sint16 y);

	void setX(Sint16 x);
	Sint16 getX();

	void setY(Sint16 y);
	Sint16 getY();

	Sint16 getArrayMiddleX();
	Sint16 getArrayMiddleY();

	Sint8 setRelX(Sint16 x);
	Sint16 getRelX();
	Sint16 getRelX(Sint16 x);

	Sint8 setRelY(Sint16 y);
	Sint16 getRelY();
	Sint16 getRelY(Sint16 y);

	void setArrayX(Sint16 x);
	Sint16 getArrayX(Sint16 x);
	Sint16 getArrayX();

	void setArrayY(Sint16 y);
	Sint16 getArrayY(Sint16 y);
	Sint16 getArrayY();

	Uint32 getFrame();
	Sint32 getNewFrame(Uint32 frame);
	SDL_Surface *getFrameSurface(Uint32 frame, imagecolors color);
	Sint16 getSurfaceX(Uint32 frame);
	Sint16 getSurfaceY(Uint32 frame);

	void startAnim();
	void stopAnim();

	CGameObject();
	~CGameObject();

	CErrorReporter er;
};

#endif // GAMEOBJECT_H
