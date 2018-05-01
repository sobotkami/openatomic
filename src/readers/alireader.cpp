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

#include "alireader.h"

#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

void CAliReader::trimRight ( string& str, string& trimChars )
{
	string::size_type pos = str.find_last_not_of ( trimChars );
	str.erase ( pos + 1 );
}

void CAliReader::trimLeft ( string& str, string& trimChars )
{
	string::size_type pos = str.find_first_not_of ( trimChars );
	str.erase ( 0, pos );
}

void CAliReader::trim ( string& str, string& trimChars )
{
	trimRight ( str, trimChars );
	trimLeft ( str, trimChars );
}


CAliReader::CAliReader()
{
	whiteSpaces.assign ( " \f\n\r\t" );
	er.setLevel( LOG_ERROR );
}


/**
 * Function read and parse ali file.
 * @param filename File name of ali file. Default is MASTER.ALI.
 * @return Returns 0 if success, non-zero value if failure.
 */
Uint8 CAliReader::read ( string filename )
{
	ifstream is;
	string line;

	is.open ( filename.c_str(), ios::binary );

	if ( is.is_open() )
	{
		while ( is.good() )
		{
			while ( getline ( is, line ) )
			{
				if ( line[0] == '-' )
				{
					line.erase ( 0, 1 ); // delete "-" from filename
					trimRight ( line,  whiteSpaces );
					transform ( line.begin(), line.end(), line.begin(), ::toupper ); // transform to upper case
					anilist.push_back ( line ); // add to list
					
					er.report( LOG_INFO, _("%s: Adding file '%s' to animation list.\n"), AT, line.c_str() );
				}
			}
		}
		is.close();
		return 0;
	}
	else
	{
		er.report( LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, filename.c_str() );
		return 1;
	}

}

/**
 * Returns the number of files to process.
 * @return Number of files.
 */
Uint16 CAliReader::getCount()
{
	return anilist.size();
}

/**
 * Returns the file name, an indexed parameter. Numbered from 0.
 * @param index Index of filename
 * @return Returns filename.
 */
string CAliReader::getItem ( Uint16 index )
{
	return anilist[index];
}

