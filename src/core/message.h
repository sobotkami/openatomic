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

#ifndef MESSAGE_H
#define MESSAGE_H

#include "../def/sdl.h"

// dropbomb
typedef struct
{
    Uint8 player;
    Uint8 x;
    Uint8 y;
    bool bomb;
} message_dropbomb_t;

// player
typedef struct
{
    Uint8 player;
    Uint8 stateRun;
    Uint8 stateLife;
    bool north; // true - down, false - up
    bool south;
    bool west;
    bool east;
} message_player_t;

typedef enum
{
    gameStart2, // start the game (aka button START!)
    gameExit2   // exit the game (aka in game key F11)
} gamestate2_t;

// game state
typedef struct
{
    gamestate2_t gamestate;
} message_gamestate_t;

typedef enum
{
    dropbomb,
    player,
    gamestate
} message_type_t;

typedef union
{
    message_dropbomb_t  mDropbomb;
    message_player_t    mPlayer;
    message_gamestate_t mGamestate;
} message_payload_t;

typedef struct
{
    message_type_t    type;
    message_payload_t payload;
} message_t;

class CMessage {
public:
    message_type_t    type;
    message_payload_t payload;
    Uint32 timestamp;

    friend bool operator< (const CMessage& lhs, const CMessage &rhs);
    friend bool operator> (const CMessage& lhs, const CMessage &rhs);

    CMessage() {}
    CMessage( message_t message, Uint32 timestamp )
    {
        this->type = message.type;
        this->payload = message.payload;
        this->timestamp = timestamp;
    }
};

#endif // MESSAGE_H
