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

#include "gameobject.h"

//extern CAnimation a_hurry;
//extern CAnimation a_xxx;
//extern CAnimation a_cursor1;
//extern CAnimation a_ring;

//extern CAnimation a_power[CPOWERUPS];
//extern CAnimation a_flame[9];
//extern CAnimation a_flameb[11];
//extern CAnimation a_tile[CFIELDS][CTILES];

//extern CAnimation a_arrow[CDIRECTIONS];
//extern CAnimation a_warp;
//extern CAnimation a_trampoline;
//extern CAnimation a_conveyor[CDIRECTIONS];

//extern CAnimation a_font;

//extern CRender render;

void CGameObject::setGamingArea (CGamingArea * ga)
{
	assert(ga != NULL);
	this->ga = ga;
}

CGameObject::CGameObject ()
{
	area.w = 640; // width
	area.h = 480; // height
	area.x = 0; // offset x
	area.y = 0; // offset y

	x = 0;
	y = 0;

	ticksStartAnim = 0;
	ticksDoneAnim = 0;

	this->ani = NULL;
	this->ga = NULL;

	animLoop = true;
	animDone = false;

	er.setLevel(LOG_INFO);
}

void CGameObject::setAni (CAnimation* ani)
{
	assert(ani != NULL);
	this->ani = ani;
}

CGameObject::~CGameObject ()
{
	stopAnim();
}

void CGameObject::startAnim ()
{
	assert(ani != NULL);
	stopAnim();
	ticksStartAnim = SDL_GetTicks();
}

void CGameObject::stopAnim ()
{
	ticksStartAnim = 0;
}

void CGameObject::setArea (earea area)
{
	tarea = area;
	switch (area)
	{
	default:
	case ea_menuMain:
		this->area.x = 313; // offset x
		this->area.y = 102; // offset y
		this->area.w = 640 - this->area.x; // width
		this->area.h = 480 - this->area.y; // height
		break;
	case ea_gameBoard:
		/* field is 40x36px, board is 15x11 */
		this->area.x = 20; // offset x
		this->area.y = 68; // offset y
		this->area.w = (GA_W - 1) * BOX_W; // width
		this->area.h = (GA_H - 1) * BOX_H; // height
		break;
	}
}

void CGameObject::setType (Uint8 type)
{
	this->type = type;
}

SDL_Rect CGameObject::getArea ()
{
	return area;
}

bool CGameObject::getAnimDone ()
{
	return animDone;
}

void CGameObject::setAnimLoop (bool loop)
{
	animDone = false;
	ticksDoneAnim = 0;
	animLoop = loop;
}

Uint32 CGameObject::getTicksDoneAnim ()
{
	return ticksDoneAnim;
}

Uint32 CGameObject::getFrame ()
{
	assert(ani != NULL);

	// delete!
//	if (ani->seq.stats.size() == 0)
//	{
//		er.report(LOG_WARNING, "%s: Count of Animation.Sequence[%s].States = %d\n", AT, ani->seq.head.name.c_str(), ani->seq.stats.size());
//		return 0;
//	}

	assert(ani->seq.stats.size() > 0);
	assert(ani->seq.head.states > 0);


	/* TODO: FIX constant 0 in state delay */

	if (ticksStartAnim > 0)
	{
		Sint32 deltaTick = SDL_GetTicks() - ticksStartAnim;

		if (animLoop)
		{
			return (deltaTick / (ani->seq.stats[0]).head.delay) % ((ani->seq.head.states));
		}
		else
		{
			if (animDone)
			{
				// freeze animation on last frame
				return (ani->seq.head.states) - 1;
			}
			else
			{
				if ((ani->seq.stats[0]).head.delay * (ani->seq.head.states - 1) < deltaTick)
				{
					animDone = true;
					ticksDoneAnim = SDL_GetTicks();
					// freeze animation on last frame
					return (ani->seq.head.states) - 1;
				}

				return (deltaTick / (ani->seq.stats[0]).head.delay) % ((ani->seq.head.states));
			}
		}




		/*
		er.report(LOG_INFO, _("%s: %d,%d,%d - [%d/%d] - %d,%d\n"),
				  AT,
				  SDL_GetTicks(), tickStartAnim, deltaTick,
				  (deltaTick / (ani->seq.stats[frame]).head.delay) % (ani->seq.head.states - 1), (ani->seq.head.states - 1),
				  (ani->seq.stats[frame]).head.delay, ((ani->seq.stats[frame]).head.delay / 10) * 10
				  );
		 */
	}
	else
	{
		return 0;
	}
}

Sint32 CGameObject::getNewFrame (Uint32 frame)
{
	static Sint32 oldFrame = -1;

	if ((Sint32) frame != oldFrame)
	{
		// new frame to render
		oldFrame = frame;
		return frame;
	}
	else
	{
		// no new
		return -1;
	}
}

/**
 * Return SDL_Surface represents surface of image in current time of animation.
 * @param frame Current frame
 * @param color Color of frame
 * @return Return SDL_Surface of current frame.
 */
SDL_Surface* CGameObject::getFrameSurface (Uint32 frame, imagecolors color)
{
	assert(ani != NULL);
	assert(ani->seq.stats.size() > 0);
	assert(ani->seq.stats[0].frams.size() > 0);
	assert(ani->seq.stats[0].frams[0].frames.size() > 0);
	assert(frame < ani->seq.stats.size());

	return (*(ani->frm))[ ani->seq.stats[frame].frams[0].frames[0].imgno ].img.im[ color ];

}

Sint16 CGameObject::getX ()
{
	return x;
}

Sint16 CGameObject::getY ()
{
	return y;
}

void CGameObject::setX (Sint16 x)
{
	//cout << "x: " << x << "(" << this->x << ")" << endl;

	this->x = x;
	//	 cout << "[" << getArrayX() << ", " << getArrayY() << "]" << endl;
}

void CGameObject::setY (Sint16 y)
{
	//cout << "y: " << y << "(" << this->y << ")" << endl;

	this->y = y;
	//	 cout << "[" << getArrayX() << ", " << getArrayY() << "]" << endl;
}

Sint16 CGameObject::getArrayMiddleX ()
{
	return getX() + BOX_W2;
}

Sint16 CGameObject::getArrayMiddleY ()
{
	return getY() + BOX_H2;
}

Sint16 CGameObject::getArrayX ()
{
	return getArrayX(getX());
}

Sint16 CGameObject::getArrayX (Sint16 x)
{
	Sint16 res = x / BOX_W;

	if (res < 0)
		return 0;

	if (res >= GA_W)
		return GA_W - 1;

	return res;
}

Sint16 CGameObject::getArrayY ()
{
	return getArrayY(getY());
}

Sint16 CGameObject::getArrayY (Sint16 y)
{
	Sint16 res = y / BOX_H;

	if (res < 0)
		return 0;

	if (res >= GA_H)
		return GA_H - 1;

	return res;
}

void CGameObject::setArrayX (Sint16 x)
{
	setX(getArrayX(x * BOX_W) * BOX_W);
}

void CGameObject::setArrayY (Sint16 y)
{
	setY(getArrayY(y * BOX_H) * BOX_H);
}

Sint16 CGameObject::getRelX ()
{
	return getX() + area.x;
}

Sint16 CGameObject::getRelX (Sint16 x)
{
	return x + area.x;
}

Sint16 CGameObject::getRelY ()
{
	return getY() + area.y;
}

Sint16 CGameObject::getRelY (Sint16 y)
{
	return y + area.y;
}

Sint8 CGameObject::setRelX (Sint16 x)
{


	if (getX() + x < 0)
	{
		setX(0);
		return -1;
	}

	if (getX() + x > area.w)
	{
		setX(area.w);
		return -1;
	}

	setX(getX() + x);

	return 0;
}

Sint8 CGameObject::setRelY (Sint16 y)
{


	if (getY() + y < 0)
	{
		setY(0);
		return -1;
	}

	if (getY() + y > area.h)
	{
		setY(area.h);
		return -1;
	}

	setY(getY() + y);

	return 0;
}

Sint8 CGameObject::moveRel (Sint8 x, Sint8 y)
{
	//cout << "moveRel(" << (int)x << ", " << (int)y << "), " << this->x << ":" << this->y << endl;

	Sint8 resX, resY = 0;
	resX = setRelX(x);
	resY = setRelY(y);

	if (resX == -1 || resY == -1)
		return -1;
	else
		return 0;
}

void CGameObject::moveAbs (Sint16 x, Sint16 y)
{
	cout << "moveAbs(" << (int)x << ", " << (int)y << "), " << this->x << ":" << this->y << endl;

	setX(x);
	setY(y);
}

Sint16 CGameObject::getSurfaceX (Uint32 frame)
{
	Uint16 imageno;
	Sint16 myX;

	if (frame >= ani->seq.stats.size())
	{
		er.report(LOG_WARNING, "%s: getSurfaceX(%d), frame = %d\n", AT, frame, ani->seq.stats.size() - 1);
		frame = ani->seq.stats.size() - 1;
	}

	imageno = ani->seq.stats[frame].frams[0].frames[0].imgno;
	myX = 19 - (*(ani->frm))[ imageno ].img.x;

	if (ani->seq.stats[frame].frams[0].frames[0].useRelPos)
		myX += ani->seq.stats[frame].frams[0].frames[0].relx;

	return myX;
}

Sint16 CGameObject::getSurfaceY (Uint32 frame)
{
	Uint16 imageno;
	Sint16 myY;

	if (frame >= ani->seq.stats.size())
	{
		er.report(LOG_WARNING, "%s: getSurfaceY(%d), frame = %d\n", AT, frame, ani->seq.stats.size() - 1);
		frame = ani->seq.stats.size() - 1;
	}

	imageno = ani->seq.stats[frame].frams[0].frames[0].imgno;
	myY = 35 - (*(ani->frm))[ imageno ].img.y;

	if (ani->seq.stats[frame].frams[0].frames[0].useRelPos)
		myY += ani->seq.stats[frame].frams[0].frames[0].rely;

	/*
		if ( myY < 0 )
			myY -= 18;*/

	if (myY > 14)
		myY -= 18;

	return myY;
}

void CGameObject::setColor (imagecolors color)
{
	this->color = color;
}

imagecolors CGameObject::getColor ()
{
	return this->color;
}
