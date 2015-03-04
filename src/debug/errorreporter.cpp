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

#include <debug/errorreporter.h>
#include <cstdarg>
#include <cstdio>

int CErrorReporter::report(logLevel level, const char* fmt, ... )
{
#ifndef NDEBUG
    va_list args;
    va_start ( args, fmt );
    if ( level <= this->level )
    {
        if ( LOG_ERROR )
        {
            return vfprintf ( stderr, fmt, args );
        }
        else if ( LOG_WARNING )
        {
            return vfprintf ( stderr, fmt, args );
        }
        else if ( LOG_INFO )
        {
            return vfprintf ( stdout, fmt, args );
        }
        else
        {
            return -666;
        }
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

void CErrorReporter::setLevel(logLevel level)
{
    this->level = level;
}

CErrorReporter::CErrorReporter()
{
    this->level = LOG_NOTHING;
}
