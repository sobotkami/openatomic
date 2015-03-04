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

#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

#include <SDL/SDL_stdinc.h>
#include <ctime>

#include <iostream>
using namespace std;

#define MSG_TEXT_MAXLEN 160
#define MSG_NODE_MAXLEN 30

#define PROTOCOL_VERSION 5

// --- INVENTORY --------------------------

// powerups
typedef enum
{
    bomb,
    flame,
    disease,
    kicker,
    skate,
    punch,
    grab,
    spooge,
    goldflame,
    trigger,
    jelly,
    disease3
} powerup_t;

// inventory
typedef struct
{
    Uint8 player;
    powerup_t powerup;
    Sint8 value;
} msg_inventory_t;

// --- BOMB -------------------------------
// bomb state
typedef enum
{
    bombDrop,
    bombExplode,
    bombDud,
    bombMove
} bombstate_t;

// bomb
typedef struct
{
    Uint8 player;
    Uint8 x; // abs X
    Uint8 y; // abs Y
    Uint16 index; // bomb index (usage: explode)
    bombstate_t state;
} msg_bomb_t;

// --- PLAYER -----------------------------
// players state of life (L*)
typedef enum
{
    playerAlive,
    playerDies,
    playerDead
} playerlife_t;

// players state (X*)
typedef enum
{
    playerStand,
    playerWalk,
    playerBlocked
} playerstate_t;

// player
typedef struct
{
    Uint8 player;
    Sint16 x1; // abs X
    Sint16 y1; // abs Y
    Sint16 x2; // new abs X
    Sint16 y2; // new abs Y
    playerstate_t state;
    playerlife_t life;
} msg_player_t;

// --- GAME -------------------------------
// game state
typedef enum
{
    gameStart, // start the match
    gameSync, // sync field, players, etc
    gameResults, // game results
    gameExit // exit the match
} gamestate_t;

// game
typedef struct
{
    gamestate_t gamestate;
} msg_gamestate_t;

// field
typedef struct
{
    Uint8 x;
    Uint8 y;
    Sint8 occupancy; // def T* ; prazdne|odpalitelne|neodpalitelne
    Sint8 powerup; // def P* ; cislo mozneho powerup, -1 nic
} msg_field_t;

// player position
typedef struct
{
    Uint8 player;
    Uint8 x;
    Uint8 y;
} msg_pldefpos_t;

// --- TEXT -------------------------------
typedef struct
{
    char msg[MSG_TEXT_MAXLEN];
} msg_text_t;

// --- HELLO ------------------------------
typedef struct
{
    char node[MSG_NODE_MAXLEN];
    Uint32 version;
} msg_hello_t;

// --- PLAYERS MANAGEMENT -----------------
typedef enum
{
    typeNone,
    typeKey0,
    typeKey1,
    typeAi
} plmantype_t;

typedef enum
{
    nodeSetup,
    nodeQuestion
} plmanstate_t;

typedef struct
{
    char node[MSG_NODE_MAXLEN];
    Uint8 player;
    plmantype_t type;
    plmanstate_t state;
} msg_plman_t;

// --- MESSAGE-----------------------------
typedef enum
{
    typeBomb,
    typePlayer,
    typeGamestate,
    typeText,
    typeHello,
    typePlMan,
    typeField,
    typePlDefPos,
    typeInventory
} msg_type_t;

typedef union
{
    msg_bomb_t      mBomb;
    msg_player_t    mPlayer;
    msg_gamestate_t mGamestate;
    msg_text_t      mText;
    msg_hello_t     mHello;
    msg_plman_t     mPlMan;
    msg_field_t     mField;
    msg_pldefpos_t  mPlDefPos;
    msg_inventory_t mInventory;
} msg_payload_t;

typedef struct
{
    Uint32 timestamp;
    msg_type_t    type;
    bool forward;
    char node[MSG_NODE_MAXLEN];
    msg_payload_t payload;
} msg_t;

#endif // NETWORKMESSAGE_H
