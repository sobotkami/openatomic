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

#ifndef SEQPARSER_H
#define SEQPARSER_H

#include <string>
#include <vector>
#include <map>
#include "../def/sdl.h"
#include <iostream>
#include <sstream>
#include "../readers/anireader.h"
#include "animation.h"
#include <debug/errorreporter.h>
#include "../global.h"

enum StringValue
{
    eunknown = 0, /* must by zero --> std::map require */
    e0 = 100,
    e1 = 101,
    e2 = 102,
    e3 = 103,
    e4 = 104,
    e5 = 105,
    e6 = 106,
    e7 = 107,
    e8 = 108,
    e9 = 109,
    e10 = 110,
    e11 = 111,
    e12 = 112,
    e13 = 113,
    e14 = 114,
    e15 = 115,
    e16 = 116,
    e17 = 117,
    e18 = 118,
    e19 = 119,
    e20 = 120,
    e21 = 121,
    e22 = 122,
    e23 = 123,
    e24 = 124,
    estand,
    ewalk,
    ekick,
    edie,
    eshadow,
    ecornerhead,
    espin,
    eflame,
    ebomb,
    epunch,
    epickup,
    etile,
    enumeric,
    eextra,
    epower,
    ering,
    exxx,
    ecursor1,
    ehurry,
    enorth,
    esouth,
    ewest,
    eeast,
    eskate,
    ekicker,
    ejelly,
    etrigger,
    egrab,
    espooge,
    egoldflame,
    edisease,
    edisease3,
    erandom,
    eregular,
    egreen,
    edud,
    ebrick,
    esolid,
    eblank,
    etrampoline,
    ewarp,
    earrow,
    econveyor,
    efont,
    ecenter,
    emidwest,
    etipwest,
    emideast,
    etipeast,
    emidnorth,
    etipnorth,
    emidsouth,
    etipsouth
};

#define PREPARE 1
#define NOTPREPARE 0


using namespace std;

class CSeqParser
{
private:
    map<string, StringValue> ms;

    CErrorReporter er;
public:
    CSeqParser();
    Uint8 assign ( sequence seq, vector<frame> * frm );

};

#endif // SEQPARSER_H
