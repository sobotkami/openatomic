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

#include <ai/aimodel.h>
#include <def/point.h>
#include <3rdparty/astar/mapsearchnode.h>
#include <vector>
using namespace std;

extern CGO_Player players[CCOLORS]; // FIX!

CAIModel::CAIModel(CGamingArea *ga)
{
    setGa(ga);
    CAIModel();
}

CAIModel::CAIModel()
{
    er.setLevel(LOG_INFO);
}

void CAIModel::addPlayer(imagecolors color, Uint8 x, Uint8 y)
{
    ostringstream stm;

    stm << "AI " << (int)color;

    aip[color].setActive(true);
    aip[color].setColor(color);
    aip[color].setName( stm.str() );
}

void CAIModel::addPlayer(imagecolors color)
{
    addPlayer(color, 0, 0);
}

void CAIModel::removePlayer(imagecolors color)
{
    aip[color].setActive(false);
}


void CAIModel::setGa(CGamingArea *ga)
{
    assert( ga != NULL );
    this->ga = ga;
    astarsearch.setGA(ga);
}


void CAIModel::doMove(CPriorityConcurrentQueue<CMessage> *q)
{
    Sint16 x, y; // boundary
    Sint16 xf, yf; // full field
    Sint16 xo, yo; // oponent

    unsigned int SearchState;

    static message_t msgPlayer;
    msgPlayer.type = player;

    message_t msgBomb;
    msgBomb.type = dropbomb;

    for(Uint8 i = 0; i < CPLAYERS; i++)
    {
        Uint8 oponent = aip[i].getOponent();


        // generate new oponent
        /// ???
        if( oponent != aip[i].getColor() && (!players[oponent].getActive() || !players[oponent].getLifeState() == LALIVE) )
        {
            er.report(LOG_INFO, "%s: %d: Oponent %d died\n", AT, aip[i].getColor(), oponent);
            oponent = aip[i].getColor();
            aip[i].setOponent((imagecolors)oponent);
        }
        if( oponent == aip[i].getColor() ) // ?? fix?
        {

            srand(SDL_GetTicks() + i + aip[i].getColor());
            //do
            //{
                oponent = rand() % CPLAYERS;
                if(oponent != aip[i].getColor() && players[oponent].getActive() && players[oponent].getLifeState() == LALIVE)
                {
                    aip[i].setOponent((imagecolors)oponent);
                }
            //} while (aip[i].getOponent() == aip[i].getColor());

        }


        // send new state
        if(aip[i].getActive() && players[i].getLifeState() == LALIVE && aip[i].getOponent() != aip[i].getColor() )
        {
            x = players[i].getArrayX(players[i].getArrayMiddleX());
            y = players[i].getArrayY(players[i].getArrayMiddleY());
            xf = players[i].getArrayX();
            yf = players[i].getArrayY();

            xo = players[aip[i].getOponent()].getArrayX(players[aip[i].getOponent()].getArrayMiddleX());
            yo = players[aip[i].getOponent()].getArrayY(players[aip[i].getOponent()].getArrayMiddleY());


            if ( xf == xo && yf == yo )
            {
                msgPlayer.payload.mPlayer.stateRun = XSTAND;
                msgPlayer.payload.mPlayer.player = i;
                msgPlayer.payload.mPlayer.stateLife = players[i].getLifeState();
                msgPlayer.payload.mPlayer.north = false;
                msgPlayer.payload.mPlayer.south = false;
                msgPlayer.payload.mPlayer.west = false;
                msgPlayer.payload.mPlayer.east = false;

                q->push (CMessage( msgPlayer, SDL_GetTicks() ));

                msgBomb.payload.mDropbomb.player = i;
                msgBomb.payload.mDropbomb.x = x;
                msgBomb.payload.mDropbomb.y = y;

                q->push (CMessage( msgBomb, SDL_GetTicks() ));
                continue;
            }

            // Create a start state
            MapSearchNode nodeStart(x,y);
            // Define the goal state
            MapSearchNode nodeEnd(xo,yo);

            if ( x == xo && y == yo )
                continue;

            ////er.report(LOG_INFO, _("%s: Searching from (%d,%d) to (%d,%d)\n"), AT, x, y, xo, yo);

            // Set Start and goal states
            astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

            do
            {
                SearchState = astarsearch.SearchStep();
            }
            while( SearchState == AStarSearch<MapSearchNode, CGamingArea>::SEARCH_STATE_SEARCHING );

            if( SearchState == AStarSearch<MapSearchNode, CGamingArea>::SEARCH_STATE_SUCCEEDED )
            {
                //er.report(LOG_INFO, _("%s: Search found goal state\n"), AT);

                MapSearchNode *node = astarsearch.GetSolutionStart();

                //er.report(LOG_INFO, _("%s: Displaying solution\n"), AT);

                node = astarsearch.GetSolutionNext();
                //node->PrintNodeInfo();

                Sint8 dx, dy;
                dx = node->GetNodePositionX() - x;
                dy = node->GetNodePositionY() - y;

                if( dx == 0 && dy == 0 )
                {
                    //playerPlaceChanged[i] = false;
                    msgPlayer.payload.mPlayer.stateRun = XSTAND;
                }
                else
                {
                    //er.report(LOG_INFO, _("%s: Next position: (%d,%d)\n"), AT, dx, dy );

                    /*
                     * 0 - left
                     * 1 - right
                     * 2 - up
                     * 3 - down
                     */

                    //playerPlaceChanged[i] = true;

                    msgPlayer.payload.mPlayer.stateRun = XWALK;
                }

                msgPlayer.payload.mPlayer.player = i;
                msgPlayer.payload.mPlayer.stateLife = players[i].getLifeState();
                msgPlayer.payload.mPlayer.north = dy < 0;
                msgPlayer.payload.mPlayer.south = dy > 0;
                msgPlayer.payload.mPlayer.west = dx < 0;
                msgPlayer.payload.mPlayer.east = dx > 0;

                q->push (CMessage( msgPlayer, SDL_GetTicks() ));

                //node = astarsearch.GetSolutionNext();
                //node->PrintNodeInfo();

                //nextPlace[i] = node->GetNodePosition();

                /*
                er.report(LOG_INFO, _("%s: Next position: abs(%d,%d), rel(%d,%d)\n"), AT,
                          node->GetNodePositionX(), node->GetNodePositionY(),
                          node->GetNodePositionX() - x, node->GetNodePositionY() - y
                          );
                          */

                /*
                for( ;; )
                {
                    node = astarsearch.GetSolutionNext();

                    if( !node )
                    {
                        break;
                    }

                    node->PrintNodeInfo();
                    steps ++;

                };
                */
                //cout << "Solution steps " << steps << endl;

                // Once you're done with the solution you can free the nodes up
                astarsearch.FreeSolutionNodes();

            }
            else if( SearchState == AStarSearch<MapSearchNode, CGamingArea>::SEARCH_STATE_FAILED )
            {
                //er.report(LOG_WARNING, _("%s: Search terminated. Did not find goal state\n"), AT);
            }

            astarsearch.EnsureMemoryFreed();
        }
    }
}
