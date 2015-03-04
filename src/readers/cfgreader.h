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

#ifndef CFGREADER_H
#define CFGREADER_H

#include <string>
using namespace std;


#include <debug/errorreporter.h>
#include <def/platform.h>
#include <def/screen.h>
#include <def/version.h>

#define CONF_VERSION VER_STRING
#define CONF_MAXPATH 32767
#define CONF_OPTFILE "bomb.cfg"

#if defined(CONF_PF_WINDOWS)
    #define CONF_ROOT "APPDATA"
    #define CONF_PATH "OpenAtomic"
    #define CONF_SEPARATOR "\\"
    #define MKDIR(X) mkdir(X)
    // for mkdir
    #include <dir.h>
    
#elif defined(CONF_PF_LINUX)
    #define CONF_ROOT "HOME"
    #define CONF_PATH ".OpenAtomic"
    #define CONF_SEPARATOR "/"
    #define MKDIR(X) mkdir(X,0777)
    // for mkdir
    #include <sys/stat.h>
    #include <sys/types.h>
    
#elif defined(CONF_PF_MACOSX)
    #define CONF_ROOT "HOME"
    #define CONF_PATH "Library/Application Support/OpenAtomic"
    #define CONF_SEPARATOR "/"
    #define MKDIR(X) mkdir(X,0777)
    // for mkdir
    #include <sys/stat.h>
    #include <sys/types.h>

#else
    #define CONF_ROOT ""
    #define CONF_PATH ""
    #define CONF_SEPARATOR "/"
    #define MKDIR(X) mkdir(X)
    
#endif

class CCfgReader
{
private:
    bool fullscreen;
    string bombversion;
    string configpath;
    string rootpath;
    int fieldNum;
    string scheme;
    bool checkConfigDirectory();
    bool saveConfiguration();

    CErrorReporter er;
public:
    string getBombVersion();
    string getConfigPath();
    string getRootPath();
    bool getFullscreen();
    int getFieldNum();
    string getScheme();
    bool readConfigFile();
//protected:
    CCfgReader();
};

#endif // CFGREADER_H
