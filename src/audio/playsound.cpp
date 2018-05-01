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
 * File:   playsound.cpp
 * Author: minji
 * 
 * Created on 17. Januar 2013, 10:36
 */

#include "playsound.h"
#include "../core/resources.h"

CPlaySound *CPlaySound::s_instance;

CPlaySound::CPlaySound()
{
	er.setLevel(LOG_INFO);
}

void CPlaySound::PlayMenuClick()
{
	Mix_PlayChannel(-1, CResources::instance()->getChooseSound(), 0);
}

void CPlaySound::PlayDie()
{
	Mix_PlayChannel(-1, CResources::instance()->getDieSound(rand() % C_DIE_SOUNDS), 0);
}

void CPlaySound::PlayExplode()
{
	Mix_PlayChannel(-1, CResources::instance()->getExplodeSound(rand() % C_EXPLODE_SOUNDS), 0);

}
