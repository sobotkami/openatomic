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


#ifndef GAMINGAREA_H
#define GAMINGAREA_H

#include <fstream>
#include <sstream>
#include <cstdlib>

#include <def/sdl.h>
#include <global.h>
#include <debug/errorreporter.h>
#include <def/players.h>
#include <def/colors.h>

using namespace std;

typedef struct FIRE
{
	Sint8 state;
	imagecolors color;
	Uint32 ticksFireup;
} t_fire;

typedef struct POWERUPS {
	Sint8 bornwith;
	Sint8 hasOverride;
	Sint8 overrideValue;
	Sint8 forbidden;
} t_powerups;

typedef struct PLAYER {
	Sint8 x;
	Sint8 y;
	Sint8 team;
} t_player;

typedef struct DIRECTION {
	Sint8 direction; // def A* ; smer
} t_direction;

typedef struct WARP {
	Sint8 x;
	Sint8 y;
} t_warp;

typedef struct FIELD {
	bool bomb; // na policku je bomba
	t_fire fire; // def F* ; na policku je ohen - cislo udava smer, -1 nic
	Sint8 occupancy; // def T* ; prazdne|odpalitelne|neodpalitelne
	Sint8 powerup; // def P* ; cislo mozneho powerup, -1 nic
	Uint8 * trampoline; // na policku je trampolina
	t_direction * arrow; // na policku je sipka
	t_direction * conveyor; // na policku je jezdici pas
	t_warp * warp; // na policku je propast
} t_field;

class CGamingArea {
private:
	// -V
	Uint16 schVersion;
	// -N
	string schName;
	// -B
	Sint8 schBrickDensity;
	// -S
	t_player schPlayers[CPLAYERS];
	// -P
	t_powerups schPowerups[CPOWERUPSR];
	// EXT-U
	Sint8 schUseArrowsSchema;
	Sint8 schUseConveyorSchema;
	Sint8 schUseTrampolineSchema;
	Sint8 schUseWarpSchema;
	// EXT-V
	Sint16 schCountOfTrampolines;
	Sint8 schRandomExitWarp;

	t_field board[GA_H][GA_W];
	Uint32 fieldNum; // type of field, eg. number

	Sint16 numRandomPowerups;

	void addArrow(Uint8 x, Uint8 y, Uint8 direction);
	void addConveyor(Uint8 x, Uint8 y, Uint8 direction);
	void addTrampoline(Uint8 x, Uint8 y);
	void addWarp(Uint8 x1, Uint8 y1, Uint8 x2, Uint8 y2);

	void genDefArrows();
	void genDefConveyors();
	void genDefTrampolines();
	void genDefWarps();

	void clearBoard();

	CErrorReporter er;

public:
	bool loadScheme(string filename);
	Uint32 getFieldNum();
	void setFieldNum(Uint32 num);

	t_field getBoardField(Uint8 x, Uint8 y);
	Sint32 getBoardFieldPath(Sint8 x, Sint8 y);
	t_player getPlayerInfo(Uint8 pno);
	void setDefPlPos(Uint8 player, Uint8 x, Uint8 y);
	t_powerups getPowerupInfo(Uint8 powerup);
	Sint8 getPowerupMax(Uint8 powerup);

	void setFieldValBomb(Uint8 x, Uint8 y, bool state);
	void setFieldValFire(Uint8 x, Uint8 y, Sint8 state, imagecolors color);
	void setFieldValPowerup(Uint8 x, Uint8 y, Sint8 powerup);
	void setFieldValOccupancy(Uint8 x, Uint8 y, Sint8 occupancy);

	Sint16 getRandomPowerups();

	CGamingArea();
	~CGamingArea();
};

#endif // GAMINGAREA_H
