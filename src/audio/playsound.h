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
 * File:   playsound.h
 * Author: minji
 *
 * Created on 17. Januar 2013, 10:36
 */

#ifndef PLAYSOUND_H
#define	PLAYSOUND_H

#include <debug/errorreporter.h>

class CPlaySound {
public:
    void PlayMenuClick();
    void PlayDie();
    void PlayExplode();
    
    static CPlaySound *instance() {
        if (!s_instance)
            s_instance = new CPlaySound;
        return s_instance;
    }
private:
        CPlaySound();
        CErrorReporter er;
        static CPlaySound *s_instance;
};

#endif	/* PLAYSOUND_H */

