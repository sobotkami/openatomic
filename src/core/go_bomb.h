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
 * File:   GO_Bomb.h
 * Author: minji
 *
 * Created on 4. Januar 2013, 13:32
 */

#ifndef GO_BOMB_H
#define	GO_BOMB_H

#include "gameobject.h"

class CGO_Bomb: public CGameObject
{
private:
	bool active;
	Uint8 stateAni;
	Uint8 stateExplode;
	Uint32 ticksActivated;
	Uint8 flameLength;
	Uint32 ticksTimeoutDefault;
	Uint32 ticksTimeout;
public:

	void setActive( bool active );
	bool getActive();

	void setFlameLength( Uint8 len );
	Uint8 getFlameLength();

	Uint32 ticksToExplode();
	void setTimeout( Uint32 timeout );

	CGO_Bomb();
};

#endif	/* GO_BOMB_H */

