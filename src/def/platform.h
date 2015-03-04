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
 * File:   platform.h
 * Author: minji
 *
 * Created on 4. Januar 2013, 22:33
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

// #define CONF_PF_WINDOWS
#define CONF_PF_LINUX
// #define CONF_PT_MACOSX

#if defined(CONF_PF_WINDOWS)
    const char platformString [] = "Windows";
#elif defined(CONF_PF_LINUX)
    const char platformString [] = "Linux";
#elif defined(CONF_PT_MACOSX)
    const char platformString [] = "Mac OS X";
#else
    const char platformString [] = "Unknown OS";
#endif

#endif	/* PLATFORM_H */

