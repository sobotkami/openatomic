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

#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>
#include <def/sdl.h>

//#include "../core/gameobject.h"
//class CGO_Bomb;
//class CGO_Player;
//#include "../core/go_bomb.h"
//#include "../core/go_player.h"
#include <global.h>
#include <debug/errorreporter.h>
#include <core/gamingarea.h>
#include <readers/cfgreader.h>
#include <readers/fonreader.h>
#include <def/screen.h>

// #include <SDL/SDL_opengl.h>

//#define SDL_SUBSYSTEMS SDL_INIT_VIDEO | SDL_INIT_TIMER



// #define NUM_BG 38 // pocet moznych pozadi; vychazi z poctu PCX pozadi v DATA/RES

// cisla novych eventu
#define REDRAW 200

using namespace std;

Uint32 timerCallbackRedraw ( Uint32 interval, void* param );


class CRender
{
private:
	Uint32 win_flags;
	SDL_Surface *screen;   // Okno
	SDL_Surface *background;  // Pozadi
	bool useOpenGl;
	CCfgReader *cfg;

	SDL_Surface * b_drawgame;
	SDL_Surface * b_bonusgame;
	SDL_Surface * b_results;
	SDL_Surface * b_mainmenu;
	SDL_Surface * b_victory[CCOLORS];
	SDL_Surface * b_glue[CBACKGROUNDS];
	//SDL_Surface * b_field[CFIELDS];
	SDL_Surface * b_team[2];

	SDL_TimerID tRedraw;

	MUTEX mutRedraw;
	pthread_mutexattr_t mutRedrawAttr;
//	 SDL_mutex *mutRedraw;
//	 pthread_mutex_t mutRedraw;

//	 vector<void *> objects;
//	 vector<GameObject>::size_type objSize;
//	 vector<GameObject>::iterator objIt;

	CErrorReporter er;

public:
	CRender();
	~CRender();
	Uint8 init ( bool useOpenGL, CCfgReader * cfg );
	void drawObject ( SDL_Surface * img, Sint16 x, Sint16 y, bool flip );
	void drawProgress ( Sint16 x, Sint16 y, Uint16 w, Uint16 h, float state );
	void drawString ( Sint16 x, Sint16 y, string str, bool flip );
	void drawString ( Sint16 x, Sint16 y, const char * str, bool flip );
	void drawString ( Sint16 x, Sint16 y, string str, imagecolors color, bool flip );
	void drawString ( Sint16 x, Sint16 y, const char * str, imagecolors color, bool flip );
	void drawLoadString ( string str );
	void drawLoadString ( const char * str );
	void drawLoadString ( string str, Uint16 status, Uint16 max );
	void drawLoadString ( const char * str, Uint16 status, Uint16 max );
	void clearScreen();
	void clearRect( SDL_Rect* rect );
	SDL_Rect getDimensionsOfScreen();
	//SDL_PixelFormat * getPixelFormat();
	void setBG( SDL_Surface * background );
	Uint8 EnableEventState( Uint8 type, int state );
	Uint8 ChangeEventState( Uint8 type );
	void PushQuitEvent();
	void PushRedrawEvent();
	void redrawScene();

	void changeAnimStateRandomPowerups( bool anim );

	CGamingArea ga;
	CFonReader font;

	Uint32 timerOperationRedrawCounter;
	Uint32 timerOperationRedraw ( Uint32 interval, void * param );
};

#endif // RENDER_H
