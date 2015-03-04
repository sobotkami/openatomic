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

#include "resreader.h"
#include <fstream>
#include <sstream>

void CResReader::trimRight ( string& str, string& trimChars )
{
    std::string::size_type pos = str.find_last_not_of ( trimChars );
    str.erase ( pos + 1 );
}

CResReader::CResReader()
{
    whiteSpaces.assign ( " \f\n\r\t" );
    this->index = 0;
    er.setLevel( LOG_ERROR );
}


string CResReader::getItem ( Uint16 index )
{
    return data[index];
}



Uint8 CResReader::read ( string filename )
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
                if ( line.size() > 1 && line[0] != ';' )
                {
                    istringstream is ( line );
                    string croppedline = "";
                    getline ( is, croppedline, ';' );
                    trimRight ( croppedline, whiteSpaces );

                    istringstream is2 ( croppedline );
                    string key = "";
                    string value = "";
                    getline ( is2, key, ',' );
                    getline ( is2, value, ',' );
                    data[ atoi( key.c_str() ) ] = value;

                    er.report( LOG_INFO, _("%s: key '%s', value '%s'\n"), AT, key.c_str(), value.c_str() );
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
