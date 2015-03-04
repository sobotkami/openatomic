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
 * File:   sdl.h
 * Author: minji
 *
 * Created on 4. Januar 2013, 22:44
 */

#ifndef SDL_H
#define	SDL_H

#include "../3rdparty/glSDL/glSDL.h"

#define SDL_SUBSYSTEMS SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO

//#define SDL_VIDEO_FLAGS SDL_SWSURFACE|SDL_GLSDL|SDL_DOUBLEBUF;
#define SDL_VIDEO_FLAGS SDL_SWSURFACE | SDL_DOUBLEBUF;

#endif	/* SDL_H */

