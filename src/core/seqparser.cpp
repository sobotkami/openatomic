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

#include "seqparser.h"

extern CAnimation a_walk[CDIRECTIONS];
extern CAnimation a_stand[CDIRECTIONS];
extern CAnimation a_kick[CDIRECTIONS];
extern CAnimation a_punch[CDIRECTIONS];
extern CAnimation a_pickup[CDIRECTIONS];
extern CAnimation a_spin;

extern CAnimation a_shadow;
extern CAnimation a_hurry;
extern CAnimation a_xxx;
extern CAnimation a_cursor1;
extern CAnimation a_ring;

extern CAnimation a_power[CPOWERUPSR];
extern CAnimation a_bomb[4];
extern CAnimation a_cornerhead[CCORNERHEAD];
extern CAnimation a_die[CDIES];
extern CAnimation a_flame[CFIRES];
extern CAnimation a_flameb[11];
extern CAnimation a_tile[CFIELDS][CTILES];

extern CAnimation a_arrow[CDIRECTIONS];
extern CAnimation a_warp;
extern CAnimation a_trampoline;
extern CAnimation a_conveyor[CDIRECTIONS];

extern CAnimation a_font;

CSeqParser::CSeqParser ()
{
	ms["0"] = e0;
	ms["1"] = e1;
	ms["2"] = e2;
	ms["3"] = e3;
	ms["4"] = e4;
	ms["5"] = e5;
	ms["6"] = e6;
	ms["7"] = e7;
	ms["8"] = e8;
	ms["9"] = e9;
	ms["10"] = e10;
	ms["11"] = e11;
	ms["12"] = e12;
	ms["13"] = e13;
	ms["14"] = e14;
	ms["15"] = e15;
	ms["16"] = e16;
	ms["17"] = e17;
	ms["18"] = e18;
	ms["19"] = e19;
	ms["20"] = e20;
	ms["21"] = e21;
	ms["22"] = e22;
	ms["23"] = e23;
	ms["24"] = e24;
	ms["stand"] = estand;
	ms["walk"] = ewalk;
	ms["kick"] = ekick;
	ms["die"] = edie;
	ms["shadow"] = eshadow;
	ms["cornerhead"] = ecornerhead;
	ms["spin"] = espin;
	ms["flame"] = eflame;
	ms["bomb"] = ebomb;
	ms["punch"] = epunch;
	ms["pickup"] = epickup;
	ms["tile"] = etile;
	ms["numeric"] = enumeric;
	ms["extra"] = eextra;
	ms["power"] = epower;
	ms["ring"] = ering;
	ms["xxx"] = exxx;
	ms["cursor1"] = ecursor1;
	ms["hurry"] = ehurry;
	ms["north"] = enorth;
	ms["south"] = esouth;
	ms["west"] = ewest;
	ms["east"] = eeast;
	ms["shadow"] = eshadow;
	ms["hurry"] = ehurry;
	ms["xxx"] = exxx;
	ms["cursor1"] = ecursor1;
	ms["skate"] = eskate;
	ms["kicker"] = ekicker;
	ms["jelly"] = ejelly;
	ms["trigger"] = etrigger;
	ms["grab"] = egrab;
	ms["spooge"] = espooge;
	ms["goldflame"] = egoldflame;
	ms["disease"] = edisease;
	ms["disease3"] = edisease3;
	ms["random"] = erandom;
	ms["regular"] = eregular;
	ms["green"] = egreen;
	ms["dud"] = edud;
	ms["brick"] = ebrick;
	ms["solid"] = esolid;
	ms["blank"] = eblank;
	ms["trampoline"] = etrampoline;
	ms["warp"] = ewarp;
	ms["arrow"] = earrow;
	ms["conveyor"] = econveyor;
	ms["font"] = efont;
	ms["center"] = ecenter;
	ms["midwest"] = emidwest;
	ms["tipwest"] = etipwest;
	ms["mideast"] = emideast;
	ms["tipeast"] = etipeast;
	ms["midnorth"] = emidnorth;
	ms["tipnorth"] = etipnorth;
	ms["midsouth"] = emidsouth;
	ms["tipsouth"] = etipsouth;
	//	ms[""] = ;

	er.setLevel(LOG_WARNING);
}

Uint8 CSeqParser::assign (sequence seq, vector<frame> *frm)
{

	istringstream iss(seq.head.name);
	vector<string> subs;

	er.report(LOG_INFO, _("%s: Processing animation '%s'.\n"), AT, seq.head.name.c_str());

	do
	{
		string sub;
		iss >> sub;
		if (sub.size() > 0)
		{
			subs.push_back(sub);
		}
	}
	while (iss);

	switch (ms[ subs[0] ])
	{
	case ewalk:
		if (ms[ subs[1] ] == enorth)
		{
			a_walk[ANORTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == esouth)
		{
			a_walk[ASOUTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ewest)
		{
			a_walk[AWEST].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == eeast)
		{
			a_walk[AEAST].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case ekick:
		if (ms[ subs[1] ] == enorth)
		{
			a_kick[ANORTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == esouth)
		{
			a_kick[ASOUTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ewest)
		{
			a_kick[AWEST].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == eeast)
		{
			a_kick[AEAST].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case estand:
		if (ms[ subs[1] ] == enorth)
		{
			a_stand[ANORTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == esouth)
		{
			a_stand[ASOUTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ewest)
		{
			a_stand[AWEST].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == eeast)
		{
			a_stand[AEAST].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case epunch:
		if (ms[ subs[1] ] == enorth)
		{
			a_punch[ANORTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == esouth)
		{
			a_punch[ASOUTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ewest)
		{
			a_punch[AWEST].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == eeast)
		{
			a_punch[AEAST].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case epickup:
		if (ms[ subs[1] ] == enorth)
		{
			a_pickup[ANORTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == esouth)
		{
			a_pickup[ASOUTH].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ewest)
		{
			a_pickup[AWEST].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == eeast)
		{
			a_pickup[AEAST].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case eshadow:
		a_shadow.setSequence(seq, frm);
		break;

	case ehurry:
		a_hurry.setSequence(seq, frm);
		break;

	case exxx:
		a_xxx.setSequence(seq, frm);
		break;

	case ecursor1:
		a_cursor1.setSequence(seq, frm);
		break;

	case ering:
		a_ring.setSequence(seq, frm);
		break;

	case espin:
		a_spin.setSequence(seq, frm);
		break;

	case enumeric:
		if (subs.size() == 2 && ms[ subs[1] ] == efont)
		{
			a_font.setSequence(seq, frm);
		}
		break;

	case epower:
		if (ms[ subs[1] ] == ebomb)
		{
			a_power[PBOMB].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == eflame)
		{
			a_power[PFLAME].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == eskate)
		{
			a_power[PSKATE].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == ekicker)
		{
			a_power[PKICKER].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == ejelly)
		{
			a_power[PJELLY].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == etrigger)
		{
			a_power[PTRIGGER].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == epunch)
		{
			a_power[PPUNCH].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == egrab)
		{
			a_power[PGRAB].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == espooge)
		{
			a_power[PSPOOGE].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == egoldflame)
		{
			a_power[PGOLDFLAME].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == edisease)
		{
			a_power[PDISEASE].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == edisease3)
		{
			a_power[PDISEASE3].setSequence(seq, frm);
		}
		if (ms[ subs[1] ] == erandom)
		{
			a_power[PRANDOM].setSequence(seq, frm);
		}
		break;

	case ebomb:
		if (subs.size() > 3 && ms[ subs[1] ] == eregular && ms[ subs[2] ] == egreen && ms[ subs[3] ] == edud)
		{
			a_bomb[BDUD].setSequence(seq, frm);
			return PREPARE;
		}
		if (subs.size() > 2 && ms[ subs[1] ] == eregular && ms[ subs[2] ] == egreen)
		{
			a_bomb[BREG].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == ejelly)
		{
			a_bomb[BJEL].setSequence(seq, frm);
			return PREPARE;
		}
		if (ms[ subs[1] ] == etrigger)
		{
			a_bomb[BTRI].setSequence(seq, frm);
			return PREPARE;
		}
		break;

	case edie:
		if (subs.size() > 2 && ms[ subs[1] ] == egreen)
		{
			a_die[ ms[ subs[2] ] - 101 ].setSequence(seq, frm); /* die = 1 - 24 */
			return PREPARE;
		}
		break;

	case ecornerhead:
		if (subs.size() > 1)
		{
			//			 cout << subs[1] << ", " << ms[ subs[1] ] - 100 << endl << flush;

			a_cornerhead[ ms[ subs[1] ] - 100 ].setSequence(seq, frm); /* cornerhead = 0 - 12 */
			return PREPARE;
		}
		break;

	case eflame:
		if (subs.size() > 2)
		{
			if (ms[ subs[1] ] == ebrick)
			{
				a_flameb[ ms[ subs[2] ] - 100 ].setSequence(seq, frm); /* flame brick = 0 - 10 */
			}

			if (ms[ subs[1] ] == ecenter && ms[ subs[2] ] == egreen)
			{
				a_flame[FCENTER].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == emidwest && ms[ subs[2] ] == egreen)
			{
				a_flame[FMIDWEST].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == etipwest && ms[ subs[2] ] == egreen)
			{
				a_flame[FTIPWEST].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == emideast && ms[ subs[2] ] == egreen)
			{
				a_flame[FMIDEAST].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == etipeast && ms[ subs[2] ] == egreen)
			{
				a_flame[FTIPEAST].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == emidnorth && ms[ subs[2] ] == egreen)
			{
				a_flame[FMIDNORTH].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == etipnorth && ms[ subs[2] ] == egreen)
			{
				er.report(LOG_INFO, _("%s: Process NORTH TIP!\n"), AT);
				a_flame[FTIPNORTH].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == emidsouth && ms[ subs[2] ] == egreen)
			{
				er.report(LOG_INFO, _("%s: Process SOUTH MID!\n"), AT);
				a_flame[FMIDSOUTH].setSequence(seq, frm);
				return PREPARE;
			}

			if (ms[ subs[1] ] == etipsouth && ms[ subs[2] ] == egreen)
			{
				er.report(LOG_INFO, _("%s: Process SOUTH TIP!\n"), AT);
				a_flame[FTIPSOUTH].setSequence(seq, frm);
				return PREPARE;
			}

		}
		break;

	case etile:
		if (subs.size() > 2)
		{
			if ((ms[ subs[1] ] - 100) < 0)
			{
				/* editovaci animace, -1 v indexu -- ignorovano, prozatim */
				er.report(LOG_WARNING, _("%s: Editing animation - ignored.\n"), AT);
				break;
			}

			if (ms[ subs[2] ] == ebrick)
			{
				a_tile[ ms[ subs[1] ] - 100 ][TBRICK].setSequence(seq, frm);
			}
			if (ms[ subs[2] ] == esolid)
			{
				a_tile[ ms[ subs[1] ] - 100 ][TSOLID].setSequence(seq, frm);
			}
			if (ms[ subs[2] ] == eblank)
			{
				a_tile[ ms[ subs[1] ] - 100 ][TBLANK].setSequence(seq, frm);
			}
		}
		break;

	case eextra:
		if (subs.size() == 2)
		{
			if (ms[ subs[1] ] == etrampoline)
			{
				a_trampoline.setSequence(seq, frm);
			}
		}
		if (subs.size() > 2)
		{
			if (ms[ subs[1] ] == ewarp)
			{
				if (ms[ subs[2] ] - 100 == 1)
				{
					a_warp.setSequence(seq, frm);
				}
			}
			if (ms[ subs[1] ] == earrow)
			{
				if (ms[ subs[2] ] == enorth)
				{
					a_arrow[ANORTH].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == esouth)
				{
					a_arrow[ASOUTH].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == ewest)
				{
					a_arrow[AWEST].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == eeast)
				{
					a_arrow[AEAST].setSequence(seq, frm);
				}
			}

			if (ms[ subs[1] ] == econveyor)
			{
				if (ms[ subs[2] ] == enorth)
				{
					a_conveyor[ANORTH].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == esouth)
				{
					a_conveyor[ASOUTH].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == ewest)
				{
					a_conveyor[AWEST].setSequence(seq, frm);
				}
				if (ms[ subs[2] ] == eeast)
				{
					a_conveyor[AEAST].setSequence(seq, frm);
				}
			}
		}
		break;

	case eunknown:
		er.report(LOG_WARNING, _("%s: Unused animation '%s'.\n"), AT, seq.head.name.c_str());
		break;
	default:
		er.report(LOG_ERROR, _("%s: Incorrect implementation (or no implementation) of animation '%s'!\n"), AT, seq.head.name.c_str());
		break;
	}


	return NOTPREPARE;

}

