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
 * File:   files.h
 * Author: minji
 *
 * Created on 5. Januar 2013, 22:31
 */

#ifndef FILES_H
#define	FILES_H

#ifndef DATA_DIR
#define DATA_DIR ""
#endif

#define PATH_DIR_GUI        DATA_DIR "gui/"
#define PATH_DIR_SOUNDS     DATA_DIR "sounds/"
#define PATH_DIR_SCHEMES    DATA_DIR "DATA/SCHEMES/"
#define PATH_DIR_ANI        DATA_DIR "DATA/ANI/"
#define PATH_DIR_RES        DATA_DIR "DATA/RES/"

#define PATH_FILE_MASTERALI PATH_DIR_ANI "MASTER.ALI"
#define PATH_FILE_GUI_XML   PATH_DIR_GUI "open-atomic.xml"
#define PATH_FILE_GUI_FONT  PATH_DIR_GUI "font.png"



#endif	/* FILES_H */

