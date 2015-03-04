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

#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include "../readers/anireader.h"
#include <debug/errorreporter.h>

using namespace std;

class CAnimation
{
private:
    Uint16 aframe; // aktualni snimek

    CErrorReporter er;
public:
    sequence seq;
    vector<frame> *frm;
    CAnimation();
    void setSequence ( sequence seq, vector<frame> *frm );
    Uint16 getStatCount ();
};

#endif // ANIMATION_H
