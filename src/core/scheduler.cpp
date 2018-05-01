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

#include "scheduler.h"

CScheduler::CScheduler()
{
	calSize = cal.size();

	er.setLevel( LOG_INFO );
}

void CScheduler::nextEvent()
{
	it = cal.begin();
	SDL_RemoveTimer ( animtimer );
	animtimer = SDL_AddTimer ( it->time - SDL_GetTicks(), timerCallback, it->schItem );
}

void CScheduler::printItems()
{
	unsigned i = 0;
	
	calSize = cal.size();
	for ( i = 0; i < calSize; ++i )
	{
		er.report( LOG_INFO, _("%s: Item #%d - time: %d, action: %d\n"), AT, i, cal[i].time, cal[i].schItem );
	}
}

long int CScheduler::binarySearch ( Uint32 time )
{
	unsigned mid = 0;

	unsigned low = 0;
	unsigned high = calSize;

	while ( low < high )
	{
		mid = ( low + high ) / 2;
		if ( cal[mid].time < time )
			low = mid + 1;
		else
			high = mid;
	}

	return low;
}


int CScheduler::addItem ( schRecord* item )
{
	unsigned pos = 0;

	calSize = cal.size();

	/* prvni polozka, musime pustit casovac */
	if ( calSize == 0 )
	{
		cal.push_back ( *item );
		animtimer = SDL_AddTimer ( item->time - SDL_GetTicks(), timerCallback, item->schItem );
		return 1;
	}

	it = cal.begin();

	/* nastane driv, nez prvni polozka v kalendari */
	if ( item->time < cal[0].time )
	{
		it = cal.insert ( it, *item );
		nextEvent();
		return 2;
	}

	pos = binarySearch ( item->time );

	/* vloz na spravnou pozici v kalendari (serazeno podle casu) */
	if ( pos > calSize )
	{
		cal.push_back ( *item );
	}
	else
	{
		it += pos;
		it = cal.insert ( it, *item );
	}
	
	return 3;
}

Uint32 timerCallback ( Uint32 interval, void* param )
{
	return ( ( CScheduler * ) param )->timerOperation ( interval, param );
}


Uint32 CScheduler::timerOperation ( Uint32 interval, void* param )
{
	er.report( LOG_INFO, _("%s: timerOperation(): %d\n"), AT, interval );
	( ( CSchedulerItem * ) param )->wakeup ( interval );
	cal.erase ( cal.begin() );
	nextEvent();
	return interval;
}


CSchedulerItem::CSchedulerItem()
{
//	 cout << "SchedulerItem()" << endl << flush;
}

void CSchedulerItem::wakeup( Uint32 interval )
{
	cout << "wakeup()" << endl << flush;
//	 item.time = delay + SDL_GetTicks();
//	 addItem( &item );
}

