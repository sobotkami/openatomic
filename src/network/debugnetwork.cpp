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

#include "debugnetwork.h"

#include <SDL/SDL.h>

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace std;

void CDebugNetwork::printMessagePayload( const char * pre, msg_t &m )
{
	assert( pre != NULL );


	cout << "[" << SDL_GetTicks() << ", " << setw(6) << SDL_GetTicks() - m.timestamp << "] (" << pre << "): ";
	switch(m.type)
	{
	case typeBomb:
		switch(m.payload.mBomb.state)
		{
		case bombDrop:
			cout << "[bomb]["<< m.node <<"] Player " << (Uint32)m.payload.mBomb.player
				 << " dropped the bomb at " << (Uint32)m.payload.mBomb.x
				 << "," << (Uint32)m.payload.mBomb.y << endl;
			break;
		case bombExplode:
			cout << "[bomb]["<< m.node <<"] Bomb " << (Uint32)m.payload.mBomb.index
				 << " exploded!" << endl;
			break;
		case bombDud:
			cout << "[bomb]["<< m.node <<"] Bomb dud" << endl;
			break;
		case bombMove:
			cout << "[bomb]["<< m.node <<"] Bomb move" << endl;
			break;
		default:
			cout << "[bomb]["<< m.node <<"] Bomb unknown state" << endl;
		}

		break;

	case typePlayer:
		cout << "[player]["<< m.node <<"] Player " << (Uint32)m.payload.mPlayer.player
			 << " move from " << (Uint32)m.payload.mPlayer.x1
			 << "," << (Uint32)m.payload.mPlayer.y1
			 << " to " << (Uint32)m.payload.mPlayer.x2
			 << "," << (Uint32)m.payload.mPlayer.y2
			 << " with state: " << m.payload.mPlayer.state
			 << ", with life state: " << m.payload.mPlayer.life
			 << endl;

		break;

	case typeGamestate:
		switch(m.payload.mGamestate.gamestate)
		{
		case gameStart:
			cout << "["<< m.node <<"]Game starting" << endl;
			break;
		case gameSync:
			cout << "["<< m.node <<"]Game synchronise" << endl;
			break;
		case gameExit:
			cout << "["<< m.node <<"]Game exiting" << endl;
			break;
		case gameResults:
			cout << "["<< m.node <<"]Show results" << endl;
		};
		break;

	case typeText:
		cout << "["<< m.node <<"]TEXT: " << m.payload.mText.msg << endl;
		break;

	case typeHello:
		if (m.forward)
		{
			cout << "["<< m.node <<"]Node " << m.payload.mHello.node << " wants network protocol version." << endl;
		}
		else
		{
			cout << "["<< m.node <<"]Node " << m.payload.mHello.node << ": network protocol version is "
				 << m.payload.mHello.version
				 << endl;
		}

		break;

	case typePlMan:
		if(m.forward)
		{
			cout << "["<< m.node <<"]Node " << m.payload.mPlMan.node << " wants ";
			if (m.payload.mPlMan.state == nodeSetup)
			{
				cout << "setup";
			}
			else if (m.payload.mPlMan.state == nodeQuestion)
			{
				cout << "question";
			}
			cout << " player " << (Uint32)m.payload.mPlMan.player
				 << " as type " << (Uint32)m.payload.mPlMan.type
				 << endl;
		}
		else
		{
			cout << "["<< m.node <<"]Node " << m.payload.mPlMan.node << " ";
			if (m.payload.mPlMan.state == nodeSetup)
			{
				cout << "setup";
			}
			else if (m.payload.mPlMan.state == nodeQuestion)
			{
				cout << "question";
			}
			cout << " player " << (Uint32)m.payload.mPlMan.player
				 << " as type " << (Uint32)m.payload.mPlMan.type
				 << endl;
		}
		break;

	case typeField:
		if (m.forward)
		{
			cout << "["<< m.node <<"]Node " << m.node << " wants update field " << (int)m.payload.mField.x << ":" << (int)m.payload.mField.y << " with occupancy: " << (int)m.payload.mField.occupancy << ", powerup: " << (int)m.payload.mField.powerup << endl;
		}
		else
		{
			cout << "["<< m.node <<"]Node " << m.node << " updates field " << (int)m.payload.mField.x << ":" << (int)m.payload.mField.y << " with occupancy: " << (int)m.payload.mField.occupancy << ", powerup: " << (int)m.payload.mField.powerup << endl;
		}

		break;

	case typePlDefPos:
		if (m.forward)
		{
			cout << "["<< m.node <<"]Node " << m.node << " wants to setup player " << (int)m.payload.mPlDefPos.player << " to position " << (int)m.payload.mPlDefPos.x << ":" << (int)m.payload.mPlDefPos.y << endl;
		}
		else
		{
			cout << "["<< m.node <<"]Node " << m.node << " setup player " << (int)m.payload.mPlDefPos.player << " to position " << (int)m.payload.mPlDefPos.x << ":" << (int)m.payload.mPlDefPos.y << endl;
		}
		break;

	case typeInventory:
		cout << "[player]["<< m.node <<"] Player " << (Uint32)m.payload.mInventory.player << " has powerup: " << (Uint32)m.payload.mInventory.powerup << ", value: " << (Sint32)m.payload.mInventory.value << endl;
		break;

	default:
		cout << "["<< m.node <<"]type: " << m.type << endl;
	};

	cout << flush;

}
