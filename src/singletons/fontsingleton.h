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
 * File:   fontsingleton.h
 * Author: minji
 *
 * Created on 6. Januar 2013, 19:22
 */

#ifndef FONTSINGLETON_H
#define	FONTSINGLETON_H

#include <def/sdl.h>
#include <debug/errorreporter.h>
#include <readers/fonreader.h>
#include <def/files.h>

class CFontSingleton {
public:

	static CFontSingleton *instance() {
		if (!s_instance)
			s_instance = new CFontSingleton;
		return s_instance;
	}

	CFonReader font;
private:

	CFontSingleton() {
		er.setLevel(LOG_INFO);
		er.report(LOG_INFO, _("%s: CFontSingleton()\n"), AT);

		if (font.read(DATA_DIR "FONT6.FON"))
			throw;
	}

	static CFontSingleton *s_instance;

	CErrorReporter er;


};

#endif	/* FONTSINGLETON_H */

