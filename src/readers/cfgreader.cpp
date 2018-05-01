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

#include "cfgreader.h"
#include "../def/version.h"

#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
using namespace std;

CCfgReader::CCfgReader()
{
	char root[CONF_MAXPATH] = "";
	char *appdata = NULL;
	string path;

	bombversion.assign(VER_STRING);

	if ( ( appdata = getenv ( CONF_ROOT ) ) == NULL )
	{
		path.assign("");
	}
	else
	{
		path.assign(appdata);
	}

	configpath = path + string(CONF_SEPARATOR) + string(CONF_PATH) + string(CONF_SEPARATOR);
	rootpath.assign( getcwd(root, CONF_MAXPATH) );

	/* Vychozi hodnoty */
	fullscreen = false;
	fieldNum = -1;
	scheme = "X.SCH";

	er.setLevel( LOG_WARNING );

}

string CCfgReader::getBombVersion()
{
	return bombversion;
}

string CCfgReader::getConfigPath()
{
	return configpath;
}

string CCfgReader::getRootPath()
{
	return rootpath;
}

bool CCfgReader::getFullscreen()
{
	return fullscreen;
}

int CCfgReader::getFieldNum()
{
	return fieldNum;
}

string CCfgReader::getScheme()
{
	return scheme;
}



bool CCfgReader::checkConfigDirectory()
{
	if ( chdir( configpath.c_str() ) == -1 )
	{
		if ( errno == EACCES )
		{ // directory exists, but we have no permission
			er.report( LOG_ERROR, _("%s: Couldn't enter directory %s: %s.\n"), AT, configpath.c_str(), strerror(errno) );
			return false;
		}
		else
		{
			if ( MKDIR( configpath.c_str() ) != 0 )
			{
				er.report( LOG_ERROR, _("%s: Unable to create directory %s: %s.\n"), AT, configpath.c_str(), strerror(errno) );
				return false;
			}

			if ( chdir( configpath.c_str() ) == -1 )
			{
				er.report( LOG_ERROR, _("%s: Couldn't enter directory %s: %s.\n"), AT, configpath.c_str(), strerror(errno) );
				return false;
			}

			if ( chdir( rootpath.c_str() ) == -1 )
			{
				er.report( LOG_ERROR, _("%s: Couldn't enter directory %s: %s.\n"), AT, rootpath.c_str(), strerror(errno) );
				return false;
			}

			return true;
		}
	}

	if ( chdir( rootpath.c_str() ) == -1 )
	{
		er.report( LOG_ERROR, _("%s: Couldn't enter directory %s: %s.\n"), AT, rootpath.c_str(), strerror(errno) );
		return false;
	}

	return true;
}

bool CCfgReader::readConfigFile()
{
	string whiteSpaces = " \f\n\r\t";

	if ( checkConfigDirectory() )
	{
		ifstream is;
		string line;
		string filename = configpath + string(CONF_OPTFILE);

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
						croppedline.erase( croppedline.find_last_not_of(whiteSpaces) + 1 );

						istringstream is2 ( croppedline );
						string key = "";
						string value = "";
						getline ( is2, key, '=' );
						getline ( is2, value, '=' );

						er.report( LOG_INFO, _("%s: key '%s', value '%s'\n"), AT, key.c_str(), value.c_str() );

						if ( key == "fullscreen" )
						{
							fullscreen = atoi( value.c_str() ) ? true : false;
						}

						if ( key == "fieldnum" )
						{
							fieldNum = atoi( value.c_str() );
						}

						if ( key == "scheme" )
						{
							scheme = value;
						}
					}
				}
			}
			is.close();
			return true;
		}
		else
		{
			er.report( LOG_INFO, _("%s: Save default settings.\n"), AT );
			return saveConfiguration();
		}
	}
	else
	{
		er.report( LOG_ERROR, _("%s: Problem with creating/reading directory.\n"), AT );
		return false;
	}
}

bool CCfgReader::saveConfiguration()
{
	string filename = configpath + string(CONF_OPTFILE);

	ofstream os ( filename.c_str() );

	if (os.is_open())
	{
		er.report( LOG_WARNING, _("%s: Saving defaults in the configuration file.\n"), AT );

		os << "; OpenAtomic configuration file" << endl;
		os << "" << endl;
		os << "; 0 = window mode, 1 = fullscreen mode" << endl;
		os << "fullscreen=0" << endl;
		os << "" << endl;
		os << "; -1 = random field in game, or 0 - 10 for field num" << endl;
		os << "fieldnum=-1" << endl;
		os << "" << endl;
		os << "; name of map scheme file" << endl;
		os << "scheme=X.SCH" << endl;

		os.close();
		return true;
	}
	else
	{
		er.report( LOG_ERROR, _("%s: Unable to write to the default configuration file.\n"), AT );
		return false;
	}
}
