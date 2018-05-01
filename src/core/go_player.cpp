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
 * File:   go_player.cpp
 * Author: minji
 * 
 * Created on 4. Januar 2013, 13:39
 */

#include "go_player.h"

#include "../def/players.h"

extern CAnimation a_walk[CDIRECTIONS];
extern CAnimation a_stand[CDIRECTIONS];
extern CAnimation a_kick[CDIRECTIONS];
extern CAnimation a_punch[CDIRECTIONS];
extern CAnimation a_pickup[CDIRECTIONS];
extern CAnimation a_spin;

extern CAnimation a_cornerhead[CCORNERHEAD];
extern CAnimation a_die[CDIES];

extern CAnimation a_shadow;

//extern CRender render;

CGO_Player::CGO_Player ()
{
	ani = NULL;
	shadow = &a_shadow;
	setArea(ea_gameBoard);
	moveAbs(0, 0);

	dx = CGO_Player::dy = 3;
	setActive(false);
	setLifeState(LALIVE);
	setAniDirState(SSTAND, ASOUTH);
	setRunState(XSTAND);
	setColor(cred);
	setType(GOT_PLAYER);
	setName("Unnamed Player");
	setTeam(TEAM_RED);
	dropedBombs = 0;

	powerups[PBOMB ] = BWBOMB;
	powerups[PFLAME ] = BWFLAME;
	powerups[PSKATE ] = BWSKATE;
	powerups[PKICKER ] = BWKICKER;
	powerups[PJELLY ] = BWJELLY;
	powerups[PTRIGGER ] = BWTRIGGER;
	powerups[PPUNCH ] = BWPUNCH;
	powerups[PGRAB ] = BWGRAB;
	powerups[PSPOOGE ] = BWSPOOGE;
	powerups[PGOLDFLAME] = BWGOLDFLAME;
	powerups[PDISEASE ] = BWDISEASE;
	powerups[PDISEASE3 ] = BWDISEASE3;
}

void CGO_Player::move(Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life)
{

	// x and y priorites of change animation
	bool changeX = false;
	bool changeY = false;

	/*
	cout << "PLAYER \"" << this->playerName << "\" move from " <<
			x1 << ":" << y1 << " to " <<
			x2 << ":" << y2 << " Direction(s): ";
	*/


	if (x2 < x1)
	{
		setAniDirState(SWALK, AWEST);
		changeX = true;
		//cout << "LEFT ";
	}

	if (x2 > x1 && !changeX)
	{
		setAniDirState(SWALK, AEAST);
		//cout << "RIGHT ";
	}

	if (y2 < y1)
	{
		setAniDirState(SWALK, ANORTH);
		changeY = true;
		//cout << "UP ";
	}

	if (y2 > y1 && !changeY)
	{
		setAniDirState(SWALK, ASOUTH);
		//cout << "DOWN ";
	}

	if (x1 == x2 && y1 == y2)
	{
		setAniDirState(SSTAND, ANONE);
		//cout << "STAND ";
	}

	//cout << endl;

	setX(x2);
	setY(y2);
}

void CGO_Player::incPowerup (Sint32 powerup)
{
	assert(ga != NULL);
	if (powerup >= 0 && powerup < CPOWERUPSR)
	{
		if (powerup == PRANDOM)
		{
			powerup = rand() % (CPOWERUPS - 1);
		}

		powerups[ powerup ]++;

		Sint8 pu = ga->getPowerupMax(powerup);
		if (powerups[powerup] > pu)
		{
			powerups[powerup] = pu;
			er.report(LOG_WARNING, _("%s: Players have too powerups: %d (Sizing down at %d)\n"), AT, powerups[powerup], pu);
		}

		er.report(LOG_INFO, _("%s: Wohoo, player '%s' found powerup #%d!\n"), AT, playerName.c_str(), powerup);
	}
	else
	{
		er.report(LOG_WARNING, _("%s: Powerup no out of range: %d\n"), AT, powerup);
	}
}

void CGO_Player::setPowerup (Sint32 powerup, Sint8 value)
{
	if (powerup >= 0 && powerup < CPOWERUPS)
	{
		powerups[ powerup ] = value;
		er.report(LOG_INFO, _("%s: Player have now powerup %d with value %d\n"), AT, powerup, value);
	}
	else
	{
		er.report(LOG_WARNING, _("%s: Powerup no out of range: %d\n"), AT, powerup);
	}
}

Sint32 CGO_Player::getPowerup (Sint32 powerup)
{
	if (powerup >= 0 && powerup < CPOWERUPS)
	{
		return powerups[ powerup ];
	}
	return 0;
}

Sint8 CGO_Player::getSpeedX ()
{
	return dx + powerups[PSKATE] * 0.5;
}

Sint8 CGO_Player::getSpeedY ()
{
	return dy + powerups[PSKATE] * 0.5;
}

bool CGO_Player::dropBomb ()
{
	if (getLifeState() == LDEAD)
		return false;

	if (dropedBombs < powerups[PBOMB])
	{
		setDropedBombs(getDropedBombs() + 1);
		return true;
	}
	else
	{
		return false;
	}
}

void CGO_Player::setDropedBombs (Uint16 value)
{
	//dropedBombs = value;
	/*
	if (dropedBombs < 0)
	{
		dropedBombs = 0;
	}
	*/
	if (value > powerups[PBOMB])
	{
		er.report(LOG_INFO, _("%s: Correcting dropedBombs from %d to %d.\n"), AT, dropedBombs, powerups[PBOMB]);
		dropedBombs = powerups[PBOMB];
	}
	else
	{
		dropedBombs = value;
	}

	er.report(LOG_INFO, "%s: setDropedBombs(%d), sum = %d\n", AT, value, dropedBombs);
}

Uint16 CGO_Player::getDropedBombs ()
{
	return dropedBombs;
}

Sint16 CGO_Player::getSurShadowX ()
{
	return 19 - (*(shadow->frm))[ shadow->seq.stats[0].frams[0].frames[0].imgno ].img.x;
}

Sint16 CGO_Player::getSurShadowY ()
{
	return 36 - (*(shadow->frm))[ shadow->seq.stats[0].frams[0].frames[0].imgno ].img.h;
}

SDL_Surface* CGO_Player::getShadow ()
{
	return (*(shadow->frm))[ shadow->seq.stats[0].frams[0].frames[0].imgno ].img.im[0];
}

Sint8 CGO_Player::setPlayerRelXY (Sint16 x, Sint16 y)
{
	t_field f;
/*
	if( (getArrayMiddleX() + x + BOX_W2) / BOX_W >= GA_W )
		er.report(LOG_WARNING, "%s: X+ %d,%d,%d,%d\n", AT,
				  getArrayX(),
				  getArrayMiddleX(),
				  x,
				  (getArrayMiddleX() + x + BOX_W2)/BOX_W
				  );

	if( (getArrayMiddleX() + x - BOX_W2) / BOX_W < 0 )
		er.report(LOG_WARNING, "%s: X- %d,%d,%d,%d\n", AT,
				  getArrayX(),
				  getArrayMiddleX(),
				  x,
				  (getArrayMiddleX() + x - BOX_W2)/BOX_W
				  );
*/
	if (x > 0)
	{
		//if (getArrayX(getArrayMiddleX()) != getArrayX(getArrayMiddleX() + x + BOX_W2))
		{
			f = ga->getBoardField(getArrayX(getArrayMiddleX() + x + BOX_W2), getArrayY(getArrayMiddleY()));
			if (f.occupancy != TBLANK || f.bomb == true)
			{
				setX(getArrayX(getArrayMiddleX()) * BOX_W);
				return -1;
			}
			else
			{
				//				 if ( abs( (getArrayMiddleY() % BOX_H) - BOX_H2 ) >= BOX_H2 - 1 )
				//				 {
				//					 setY( getArrayY( getArrayMiddleY() ) * BOX_H );
				//					 return -5;
				//				 }
				//				 if ( getArrayX( getArrayMiddleX() + BOX_W2 ) != getArrayX( getArrayMiddleX() + x + BOX_W2 ) )
				//				 {
				//					 if ( ga->getBoardField( getArrayX( getArrayMiddleX() + x + BOX_W2 ), getArrayY( getArrayMiddleY() ) ).powerup >= 0 )
				//					 {
				//						 setPowerup( ga->getBoardField( getArrayX( getArrayMiddleX() + x + BOX_W2 ), getArrayY( getArrayMiddleY() ) ).powerup );
				//						 ga->setFieldValPowerup( getArrayX( getArrayMiddleX() + x + BOX_W2 ), getArrayY( getArrayMiddleY() ), -1 );
				//					 }
				//				 }
			}
		}
	}

	if (x < 0)
	{
		//if (getArrayX(getArrayMiddleX()) != getArrayX(getArrayMiddleX() + x - BOX_W2))
		{
			f = ga->getBoardField(getArrayX(getArrayMiddleX() + x - BOX_W2), getArrayY(getArrayMiddleY()));
			if (f.occupancy != TBLANK || f.bomb == true)
			{
				setX(getArrayX(getArrayMiddleX()) * BOX_W);
				return -1;
			}
			else
			{
				//				 if ( abs( (getArrayMiddleY() % BOX_H) - BOX_H2 ) >= BOX_H2 - 1 )
				//				 {
				//					 setY( getArrayY( getArrayMiddleY() ) * BOX_H );
				//					 return -5;
				//				 }
				//				 if ( getArrayX( getArrayMiddleX() - BOX_W2 ) != getArrayX( getArrayMiddleX() + x - BOX_W2 ) )
				//				 {
				//					 if ( ga->getBoardField( getArrayX( getArrayMiddleX() + x - BOX_W2 ), getArrayY( getArrayMiddleY() ) ).powerup >= 0 )
				//					 {
				//						 setPowerup( ga->getBoardField( getArrayX( getArrayMiddleX() + x - BOX_W2 ), getArrayY( getArrayMiddleY() ) ).powerup );
				//						 ga->setFieldValPowerup( getArrayX( getArrayMiddleX() + x - BOX_W2 ), getArrayY( getArrayMiddleY() ), -1 );
				//					 }
				//				 }
			}
		}
	}

	if (y > 0)
	{
		//if (getArrayY(getArrayMiddleY()) != getArrayY(getArrayMiddleY() + y + BOX_H2))
		{
			f = ga->getBoardField(getArrayX(getArrayMiddleX()), getArrayY(getArrayMiddleY() + y + BOX_H2));
			if (f.occupancy != TBLANK || f.bomb == true)
			{
				setY(getArrayY(getArrayMiddleY()) * BOX_H);
				return -1;
			}
			else
			{
				//				 if ( abs( (getArrayMiddleX() % BOX_W) - BOX_W2 ) >= BOX_W2 - 1 )
				//				 {
				//					 setX( getArrayX( getArrayMiddleX() ) * BOX_W );
				//					 return -5;
				//				 }
				//				 if ( getArrayY( getArrayMiddleY() + BOX_H2 ) != getArrayY( getArrayMiddleY() + y + BOX_H2 ) )
				//				 {
				//					 if ( ga->getBoardField( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y + BOX_H2 ) ).powerup >= 0 )
				//					 {
				//						 setPowerup( ga->getBoardField( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y + BOX_H2 ) ).powerup );
				//						 ga->setFieldValPowerup( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y + BOX_H2 ), -1 );
				//					 }
				//				 }
			}
		}
	}

	if (y < 0)
	{
		//if (getArrayY(getArrayMiddleY()) != getArrayY(getArrayMiddleY() + y - BOX_H2))
		{
			f = ga->getBoardField(getArrayX(getArrayMiddleX()), getArrayY(getArrayMiddleY() + y - BOX_H2));
			if (f.occupancy != TBLANK || f.bomb == true)
			{
				setY(getArrayY(getArrayMiddleY()) * BOX_H);
				return -1;
			}
			else
			{
				//				 if ( abs( (getArrayMiddleX() % BOX_W) - BOX_W2 ) >= BOX_W2 - 1 )
				//				 {
				//					 setX( getArrayX( getArrayMiddleX() ) * BOX_W );
				//					 return -5;
				//				 }
				//				 if ( getArrayY( getArrayMiddleY() - BOX_H2 ) != getArrayY( getArrayMiddleY() + y - BOX_H2 ) )
				//				 {
				//					 if ( ga->getBoardField( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y - BOX_H2 ) ).powerup >= 0 )
				//					 {
				//						 setPowerup( ga->getBoardField( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y - BOX_H2 ) ).powerup );
				//						 ga->setFieldValPowerup( getArrayX( getArrayMiddleX() ), getArrayY( getArrayMiddleY() + y - BOX_H2 ), -1 );
				//					 }
				//				 }
			}
		}
	}

	if (moveRel(x, y) != 0)
		return -1;

	if (ga->getBoardField(getArrayX(getArrayMiddleX()), getArrayY(getArrayMiddleY())).powerup >= 0)
	{
		incPowerup(ga->getBoardField(getArrayX(getArrayMiddleX()), getArrayY(getArrayMiddleY())).powerup);
		ga->setFieldValPowerup(getArrayX(getArrayMiddleX()), getArrayY(getArrayMiddleY()), -1);

		/* FIX!!!!!!!!
		if (ga->getRandomPowerups() <= 0)
		{
			render.changeAnimStateRandomPowerups(false);
		}
		else
		{
			if (ga->getRandomPowerups() == 1)
			{
				render.changeAnimStateRandomPowerups(true);
			}
		}
		 */
	}
	
	Sint16 modX, modY;
	Sint16 rdx, rdy;

	modX = getArrayMiddleX() % BOX_W;
	modY = getArrayMiddleY() % BOX_H;

	rdx = abs((getArrayMiddleX() % BOX_W) - BOX_W2) / getSpeedX();
	rdy = abs((getArrayMiddleY() % BOX_H) - BOX_H2) / getSpeedY();

	if (x != 0)
	{
		//		 if ( abs( (getArrayMiddleY() % BOX_H) - BOX_H2 ) == BOX_H2 )
		//		 {
		//			 setX( getArrayX( getArrayMiddleX() ) * BOX_W );
		//			 return -5;
		//		 }
		//		 if ( abs( (getArrayMiddleX() % BOX_W) - BOX_W2 ) == BOX_W2 )
		//		 {
		//			 setY( getArrayY( getArrayMiddleY() ) * BOX_H );
		//			 return -5;
		//		 }

		if (modY < BOX_H2)
		{
			if (getArrayMiddleY() + rdy > BOX_H2)
			{
				setY(getArrayY(getArrayMiddleY()) * BOX_H);
				return -4;
			}
			else
			{
				if (rdy == 0 && (getArrayMiddleY() % BOX_H2) > 0)
				{
					setY(getArrayY(getArrayMiddleY()) * BOX_H);
					return -3;
				}
				else
				{
					return moveRel(0, rdy);
				}
			}
		}
		else if (modY > BOX_H2)
		{
			if (getArrayMiddleY() - rdy < BOX_H2)
			{
				setY(getArrayY(getArrayMiddleY()) * BOX_H);
				return -4;
			}
			else
			{
				if (rdy == 0 && (getArrayMiddleY() % BOX_H2) > 0)
				{
					setY(getArrayY(getArrayMiddleY()) * BOX_H);
					return -3;
				}
				else
				{
					return moveRel(0, -rdy);
				}
			}
		}
	}

	if (y != 0)
	{
		//		 if ( abs( (getArrayMiddleX() % BOX_W) - BOX_W2 ) >= BOX_W2 )
		//		 {
		//			 setY( getArrayY( getArrayMiddleY() ) * BOX_H );
		//			 return -5;
		//		 }
		//		 if ( abs( (getArrayMiddleY() % BOX_H) - BOX_H2 ) >= BOX_H2 )
		//		 {
		//			 setX( getArrayX( getArrayMiddleX() ) * BOX_W );
		//			 return -5;
		//		 }

		if (modX < BOX_W2)
		{
			if (getArrayMiddleX() + rdx > BOX_W2)
			{
				setX(getArrayX(getArrayMiddleX()) * BOX_W);
				return -4;
			}
			else
			{
				if (rdx == 0 && (getArrayMiddleX() % BOX_W2) > 0)
				{
					setX(getArrayX(getArrayMiddleX()) * BOX_W);
					return -3;
				}
				else
				{
					return moveRel(rdx, 0);
				}
			}

		}
		else if (modX > BOX_W2)
		{
			if (getArrayMiddleX() - rdx < BOX_W2)
			{
				setX(getArrayX(getArrayMiddleX()) * BOX_W);
				return -4;
			}
			else
			{
				if (rdx == 0 && (getArrayMiddleX() % BOX_W2) > 0)
				{
					setX(getArrayX(getArrayMiddleX()) * BOX_W);
					return -3;
				}
				else
				{
					return moveRel(-rdx, 0);
				}
			}
		}
	}

	return -91;

	//	 if ( y != 0 )
	//	 {
	//		 if ( getX() % 40 != 0 )
	//			 setX( getArrayX() * 40 );
	//	 }

	//				 if ( x < 0 )
	//					 setY( getArrayY() * 36 + ( modY - modX ) );
	//				 else
	//					 setY( getArrayY() * 36 + ( modY - (18+modX) ) );

}

Sint8 CGO_Player::movePlayerRel (Sint8 direction, Sint8 direction2)
{
	Sint8 res = 0;
	Sint16 x1, y1, x2, y2;

	if (stateRun == XBLOCKED)
	{
		return -2;
	}
	else
	{ // XSTAND || XWALK
		//	 if ( stateRun == XSTAND || stateRun == XWALK )
		//	 {
		if (stateRun == XSTAND)
			setRunState(XWALK);

		//		 if ( this->direction != direction || stateAni == SSTAND )
		//			 setAniDirState ( SWALK, direction );

		x1 = getArrayMiddleX();
		y1 = getArrayMiddleY();

		switch (direction)
		{
		case AWEST:
			res = setPlayerRelXY(-getSpeedX(), 0);
			break;
		case AEAST:
			res = setPlayerRelXY(getSpeedX(), 0);
			break;
		case ANORTH:
			res = setPlayerRelXY(0, -getSpeedY());
			break;
		case ASOUTH:
			res = setPlayerRelXY(0, getSpeedY());
			break;
		default:
			return -90;
			break;
		}

		x2 = getArrayMiddleX();
		y2 = getArrayMiddleY();

		//		 printf("(%d,%d %d,%d) ", x1, y1, x2, y2);

		if (x1 == x2 && y1 == y2)
		{
			switch (direction2)
			{
			case AWEST:
				res = setPlayerRelXY(-getSpeedX(), 0);
				break;
			case AEAST:
				res = setPlayerRelXY(getSpeedX(), 0);
				break;
			case ANORTH:
				res = setPlayerRelXY(0, -getSpeedY());
				break;
			case ASOUTH:
				res = setPlayerRelXY(0, getSpeedY());
				break;
			default:
				if (this->direction != direction || stateAni == SSTAND)
				{
					// setAniDirState(SWALK, direction);
					// set from move()
				}
				return -10;
				break;
			}

			if (this->direction != direction2 || stateAni == SSTAND)
			{
				// setAniDirState(SWALK, direction2);
				// set from move()
				return res;
			}

			return -90;
		}
		else
		{
			//			 if ( x2 - x1 > 0 )
			//			 {
			//				 cout << "> ";
			//			 }
			//			 else if ( x2 - x1 < 0)
			//			 {
			//				 cout << "< ";
			//			 }
			//
			//			 if ( y2 - y1 > 0 )
			//			 {
			//				 cout << "v ";
			//			 }
			//			 else if ( y2 - y1 < 0 )
			//			 {
			//				 cout << "^ ";
			//			 }

			if (this->direction != direction || stateAni == SSTAND)
			{
				// setAniDirState(SWALK, direction);
				// set from move()

				return res;
			}

			return -90;
		}

		return -90;

		//		 if ( this->direction != direction || stateAni == SSTAND )
		//		 {
		//			 setAniDirState ( SWALK, direction );
		//			 return res;
		//		 }
		//		 else
		//		 {
		//			 switch (direction2)
		//			 {
		//			 case AWEST:
		//				 res = setPlayerRelXY( -getSpeedX(), 0 );
		//				 break;
		//			 case AEAST:
		//				 res = setPlayerRelXY( getSpeedX(), 0 );
		//				 break;
		//			 case ANORTH:
		//				 res = setPlayerRelXY( 0, -getSpeedY() );
		//				 break;
		//			 case ASOUTH:
		//				 res = setPlayerRelXY( 0, getSpeedY() );
		//				 break;
		//			 default:
		//				 return -90;
		//				 break;
		//			 }
		//
		//			 if ( res != -1 || this->direction != direction || stateAni == SSTAND )
		//			 {
		//				 setAniDirState ( SWALK, direction );
		//			 }
		//			 return res;
		//		 }
	}
}

void CGO_Player::setName (string playerName)
{
	this->playerName = playerName;
}

bool CGO_Player::getActive ()
{
	return active;
}

void CGO_Player::setActive (bool active)
{
	this->active = active;
}

void CGO_Player::setLifeState (Uint8 state)
{
	this->stateLife = state;
}

Uint8 CGO_Player::getLifeState ()
{
	return this->stateLife;
}

Uint8 CGO_Player::getRunState ()
{
	return this->stateRun;
}

Uint8 CGO_Player::getAniState ()
{
	return this->stateAni;
}

void CGO_Player::setRunState (Uint8 state)
{
	this->stateRun = state;
}

void CGO_Player::setRunStateSafe (Uint8 state)
{
	if (state == getRunState())
	{ // no change
		return;
	}

	switch(state)
	{
	case XSTAND:

		setRunState(state);

		if (getAniState() != SSTAND)
		{
			// setAniDirState(SSTAND, direction);
			// set from move()
		}

		break;

	case XWALK:

		setRunState(state);

		if (getAniState() != SWALK)
		{
			// setAniDirState(SWALK, direction);
			// set from move()
		}

		break;

	case XBLOCKED:

		setRunState(state);

		// move to full cell
		setArrayX( getArrayX( getArrayMiddleX()));
		setArrayY( getArrayY( getArrayMiddleY()));

		if ( getAniState() != SCORNERHEAD )
		{
			// setAniDirState(SCORNERHEAD, ASOUTH);
			// set from move()
			er.report(LOG_INFO, _("%s: setAniDirState(SCORNERHEAD, ASOUTH)\n"), AT);
		}

		break;
	};
}

void CGO_Player::setAniDirState (Uint8 state, Uint8 direction)
{
	bool directionChanged = false;

	// direction is different
	if (this->direction != direction)
	{
		// set the new direction
		if (direction < ANONE)
		{
			this->direction = direction;
			directionChanged = true;
		}

		// if direction is NONE, set the previous direction
		if (direction == ANONE)
		{
			direction = this->direction;
		}
	}

	if (this->ani != NULL && this->stateAni == state && !directionChanged)
	{
		// nothing
	}
	else
	{
		setAnimLoop(true);

		switch (state)
		{
		case SSTAND:
			stopAnim();
			this->stateAni = SSTAND;
			ani = &(a_stand[direction]);
			///////////////		render.PushRedrawEvent();
			break;
		case SWALK:
			this->stateAni = SWALK;
			stopAnim();
			ani = &(a_walk[direction]);
			startAnim();
			break;
		case SCORNERHEAD:
			this->stateAni = SCORNERHEAD;
			srand(SDL_GetTicks() + state + direction + getX() + getY() );
			stopAnim();
			ani = &(a_cornerhead[rand() % CCORNERHEAD]);
			startAnim();
			break;
		case SDIE:
			this->stateAni = SDIE;
			setAnimLoop(false); // die and freeze
			srand(SDL_GetTicks() + state + direction + getX() + getY() );
			stopAnim();
			ani = &(a_die[rand() % CDIES]);
			startAnim();
			break;
		case SPUNCH:
			this->stateAni = SPUNCH;
			stopAnim();
			ani = &(a_punch[direction]);
			startAnim();
			break;
		case SKICK:
			this->stateAni = SKICK;
			stopAnim();
			ani = &(a_kick[direction]);
			startAnim();
			break;
		case SPICKUP:
			this->stateAni = SPICKUP;
			stopAnim();
			ani = &(a_pickup[direction]);
			startAnim();
			break;
		case SSPIN:
			this->stateAni = SSPIN;
			stopAnim();
			ani = &(a_spin);
			startAnim();
			break;
		}

		//	 cout << "direction: " << (int)direction << ", stateAni: " << (int)state << endl;

	}
}

void CGO_Player::setTeam (Sint8 team)
{
	this->team = team;
}

Sint8 CGO_Player::getTeam ()
{
	return team;
}
