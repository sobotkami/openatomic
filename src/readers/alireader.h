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
#ifndef ALIREADER_H
#define ALIREADER_H

#include <string>
#include <vector>
#include <def/sdl.h>
#include <debug/errorreporter.h>

using namespace std;

class CAliReader
{
	string whiteSpaces;
	vector<string> anilist;
	void trimRight ( string& str, string& trimChars );
	void trimLeft ( string& str, string& trimChars );
	void trim ( string& str, string& trimChars );

	CErrorReporter er;
public:
	CAliReader();
	Uint8 read ( string filename );
	string getItem ( Uint16 index );
	Uint16 getCount ();
};

#endif // ALIREADER_H
