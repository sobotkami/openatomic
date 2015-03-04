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
 * File:   go_player.h
 * Author: minji
 *
 * Created on 4. Januar 2013, 13:39
 */

#ifndef GO_PLAYER_H
#define	GO_PLAYER_H

#include <string>
#include "../global.h"
#include "gameobject.h"

#include <entities/network/networkMessage.h>

using namespace std;

class CGO_Player : public CGameObject {
private:
    CAnimation *shadow;
    bool active;
    Uint8 stateAni;
    Uint8 stateRun;
    Uint8 stateLife;
    Uint8 direction;
    string playerName;
    Sint8 dx, dy;
    Sint32 powerups[CPOWERUPS];
    Uint16 dropedBombs;
    Sint8 team;
public:
    Sint8 setPlayerRelXY(Sint16 x, Sint16 y);
    Sint8 movePlayerRel(Sint8 direction, Sint8 direction2);
    Sint16 getSurShadowX();
    Sint16 getSurShadowY();
    Sint8 getSpeedX();
    Sint8 getSpeedY();

    SDL_Surface *getShadow();

    bool dropBomb();
    void setDropedBombs(Uint16 value);
    Uint16 getDropedBombs();

    void setActive(bool active);
    bool getActive();

    void setLifeState(Uint8 state);
    Uint8 getLifeState();
    
    void setRunState(Uint8 state);
    void setRunStateSafe(Uint8 state);
    Uint8 getRunState();

    void setAniDirState(Uint8 state, Uint8 direction);
    Uint8 getAniState();

    void setName(string playerName);

    void setTeam(Sint8 team);
    Sint8 getTeam();

    void incPowerup(Sint32 powerup);
    void setPowerup (Sint32 powerup, Sint8 value);
    Sint32 getPowerup(Sint32 powerup);


    void move(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life);

    CGO_Player();
};

#endif	/* GO_PLAYER_H */

