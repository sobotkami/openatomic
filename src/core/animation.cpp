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

#include "animation.h"

CAnimation::CAnimation()
{
    seq.head.name.assign ( _("(not assigned)") );
    frm = NULL;
    er.setLevel( LOG_WARNING );
}

void CAnimation::setSequence ( sequence seq, vector<frame> *frm )
{
    this->seq = seq;
    this->frm = frm;

    er.report( LOG_INFO, _("%s: Animation '%s' asociated with %d states. Number of pictures: %d.\n"), AT,
               seq.head.name.c_str(), getStatCount(), frm->size() );
}

Uint16 CAnimation::getStatCount ()
{
//     for ( Uint16 i = 0; i < seq.head.states; i++ )
//     {
//         cout << seq.stats[i].frams[0].frames[0].imgno << endl;
//     }
    return seq.head.states;
}
