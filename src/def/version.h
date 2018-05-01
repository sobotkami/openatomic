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
 * File:   version.h
 * Author: minji
 *
 * Created on 4. Januar 2013, 23:04
 */

#ifndef VERSION_H
#define	VERSION_H

#include "svnversion.h"

#ifndef STRINGIFY
#define STRINGIFY(x) #x
#endif
#ifndef TOSTRING
#define TOSTRING(x) STRINGIFY(x)
#endif


// major version number
#define VER_MAJOR	0
// minor version number
#define VER_MINOR	6
// join in one string
#define VER_STRING   TOSTRING(VER_MAJOR) "." TOSTRING(VER_MINOR) "." SVNVERSION

#endif	/* VERSION_H */

