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

#include "filereader.h"
#include <cassert>

//using namespace std;

CFileReader::CFileReader()
{
	this->fp = NULL;
	er.setLevel( LOG_ERROR );
}

CFileReader::CFileReader(SDL_RWops* fp)
{
	assert ( fp != NULL );
	CFileReader();

	this->fp = fp;
}

Uint32 CFileReader::getPos()
{
	return SDL_RWtell ( fp );
}

Uint32 CFileReader::setRelPos(Sint32 offset)
{
	return SDL_RWseek ( fp, offset, SEEK_CUR );
}

/**
 * Function reads 4 bytes from disk as an int value.
 * @return On success, the function returns the converted integral number as an int value.
 */
Sint32 CFileReader::read32()
{
	Sint32 buf = 0;
	assert ( fp != NULL );

	if ( SDL_RWread ( fp, &buf, sizeof ( buf ), 1 ) < 0 )
	{
		er.report( LOG_ERROR, _("%s: Couldn't read %d bytes from file.\n"), AT, 4 );
		exit ( 1 );
	}

	return SWAP32 ( buf );
}

/**
 * Function reads 2 bytes from disk as an short int value.
 * @return On success, the function returns the converted integral number as a short int value.
 */
Sint16 CFileReader::read16()
{
	Sint16 buf = -1;
	assert ( fp != NULL );

	if ( SDL_RWread ( fp, &buf, sizeof ( buf ), 1 ) < 0 )
	{
		er.report( LOG_ERROR, _("%s: Couldn't read %d bytes from file.\n"), AT, 2 );
		exit ( 1 );
	}

	return SWAP16 ( buf );
}

/**
 * Function read 1 byte from disk as an char value.
 * @return On success, the function returns the converted integral number as a char value.
 */
Sint8 CFileReader::read8()
{
	Sint8 buf = -1;
	assert ( fp != NULL );

	if ( SDL_RWread ( fp, &buf, sizeof ( buf ), 1 ) < 0 )
	{
		er.report( LOG_ERROR, _("%s: Couldn't read %d bytes from file.\n"), AT, 1 );
		exit ( 1 );
	}

	return buf;
}

/**
 * Function read count-times bytes from disk as an char value.
 * @param count How many bytes will be read from disk
 * @return On success, the function returns the array of bytes. On failure, returns NULL.
 */
char * CFileReader::readBytes ( Uint32 count )
{
	char * result = NULL;

	assert ( fp != NULL );

	if ( count == 0 )
	{
		er.report( LOG_WARNING, _("%s: Reading 0 bytes from file.\n"), AT );
		return NULL;
	}

	if ( ( result = ( char * ) malloc ( count * sizeof ( char ) ) ) == NULL )
	{
		er.report( LOG_ERROR, _("%s: Couldn't allocate %d bytes of memory\n"), AT, count );
		exit ( 1 );
	}

	if ( SDL_RWread ( fp, result, sizeof ( char ), count ) < 0 )
	{
		er.report( LOG_ERROR, _("%s: Couldn't read %d bytes from file.\n"), AT, count );
		exit ( 1 );
	}

	return result;
}

/**
 * Function read one byte and compare with parameter byte
 * @param byte Value, which will be tested
 * @return Returns true if the match, false if not.
 */
bool CFileReader::testByte ( Uint8 byte )
{
	return ( Uint8 ) CFileReader::read8() == byte;
}
