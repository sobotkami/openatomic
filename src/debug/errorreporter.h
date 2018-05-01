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

#ifndef DEBUG_H
#define DEBUG_H

#include <clocale>
#include <libintl.h>
//#include <string.h>

using namespace std;

/*
 * If this macro is not defined, the program will be compiled as a finished release.
 */
// #define NDEBUG

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
//#define __FILE__W (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define AT __FILE__ "," TOSTRING(__LINE__) "\t"

#define _(string) gettext(string)

typedef enum logL
{
	LOG_NOTHING,
	LOG_ERROR,
	LOG_WARNING,
	LOG_INFO
} logLevel;

class CErrorReporter
{
private:
	logLevel level;
public:
	int report ( logLevel level, const char *fmt, ... );
	void setLevel ( logLevel level );
	CErrorReporter();
};

#endif
