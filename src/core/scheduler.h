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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <def/sdl.h>
#include <debug/errorreporter.h>

using namespace std;

// definice
class CSchedulerItem;
class CScheduler;
Uint32 timerCallback ( Uint32 interval, void* param );

typedef struct
{
    CSchedulerItem *schItem;
    Uint32 time;
} schRecord;

// deklarace
class CSchedulerItem
{
private:
public:
    CSchedulerItem();
    void wakeup( Uint32 interval );
};

class CScheduler
{
private:
    vector<schRecord> cal;
    vector<schRecord>::size_type calSize;
    vector<schRecord>::iterator it;

    SDL_TimerID animtimer;
    long binarySearch ( Uint32 time );

    CErrorReporter er;
public:
    void nextEvent();
    Uint32 timerOperation ( Uint32 interval, void* param );
    void printItems();
    int addItem ( schRecord * item );
    CScheduler();
};

#endif // SCHEDULER_H
