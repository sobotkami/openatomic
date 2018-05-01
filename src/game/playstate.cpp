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

#include <def/sdl.h>
#include <def/colors.h>

#include <SDL/SDL_image.h> // need if glSDL?

#include <core/gameengine.h>
#include <core/gameresults.h>
#include <core/gameobject.h>
#include <core/gamestate.h>
#include <core/go_bomb.h>
#include <core/go_player.h>

#include <game/playstate.h>
#include <game/resultsstate.h>

#include <render/stringdraw.h>
#include <render/objectdraw.h>

#include <audio/playsound.h>




CPlayState CPlayState::m_PlayState;

extern CGO_Player players[CCOLORS];
extern CGO_Bomb bombs[CBOMBS];

extern CGameObject tiles[CFIELDS][CTILES];
extern CGameObject arrows[CDIRECTIONS];
extern CGameObject trampolines;
extern CGameObject warps;
extern CGameObject conveyors[CDIRECTIONS];
extern CGameObject powerups[CPOWERUPSR];
extern CGameObject fires[CFIRES];
extern CGameObject firesb[CFIELDS];

extern SDL_Surface * b_field[CFIELDS];

void CPlayState::Init ()
{
    er.setLevel(LOG_INFO);

    updatedField = 0;
    updatedPlDefPos = 0;

    // enable key repeat
    int resKeyRepeat = SDL_EnableKeyRepeat(1, KEYREPEAT);
    if (resKeyRepeat != 0)
    {
        er.report(LOG_WARNING, _("%s: SDL_EnableKeyRepeat(1,%d) failed.\n"), AT, KEYREPEAT);
    }

    /*
     * Background
     */
    /*
    SDL_Surface *temp = IMG_Load("DATA/RES/FIELD0.PCX");
    if (!temp)
    {
        // image failed to load
        er.report(LOG_ERROR, _("%s: IMG_Load: %s\n"), AT, IMG_GetError());
        SDL_Quit();
        return;
    }

    bg = SDL_DisplayFormat(temp);

    SDL_FreeSurface(temp);
     */
    /*************/
    CCfgReader cfg;
    cfg.readConfigFile();

    // Game Area
    ga = new CGamingArea();


    if (CGameState::instance()->getNetworkGame())
    {
        if (CGameEngine::instance()->getServer()->isNetworkRunning())
        {
            er.report(LOG_INFO, _("%s: I'm a CONTROL CLIENT!\n"), AT);
        }
        else
        {
            er.report(LOG_INFO, _("%s: I'm a CLIENT!\n"), AT);
        }
    }

    if (!CGameState::instance()->getNetworkGame())
    {
        er.report(LOG_INFO, _("%s: I'm a LOCAL CLIENT!\n"), AT);

        updatedField = GA_H * GA_W;
        updatedPlDefPos = CPLAYERS;
    }

    /*
     * Sync gaming area
     */
    if (CGameState::instance()->getNetworkGame() && CGameEngine::instance()->getServer()->isNetworkRunning())
    { // network game and we are server, so sync ..

        CGamingArea * lga = new CGamingArea();
        ostringstream stm;
        stm << PATH_DIR_SCHEMES << cfg.getScheme();
        if (lga->loadScheme(stm.str()) == false)
        {
            er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str());
        }
        stm.str("");

        t_field fi;
        for (Uint8 y = 0; y < GA_H; ++y)
        {
            for (Uint8 x = 0; x < GA_W; ++x)
            {
                fi = lga->getBoardField(x, y);
                CGameEngine::instance()->getClient()->updateField(x, y, fi.occupancy, fi.powerup);
            }
        }

        t_player player;
        for (Uint8 i = 0; i < CPLAYERS; i++)
        {
            player = lga->getPlayerInfo(i);
            CGameEngine::instance()->getClient()->setDefPlPos(i, player.x, player.y);
        }

        delete lga;



    }
    else
    { // local game

        ostringstream stm;
        stm << PATH_DIR_SCHEMES << cfg.getScheme();
        if (ga->loadScheme(stm.str()) == false)
        {
            er.report(LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str());
        }
        stm.str("");

        if (cfg.getFieldNum() == -1)
        {
            srand(time(NULL));
            ga->setFieldNum(rand() % CFIELDS);
        }
        else
        {
            ga->setFieldNum(cfg.getFieldNum());
        }
    }

    for (Uint8 i = 0; i < CBOMBS; i++)
    {
        bombs[i].setActive(false);
    }

    t_player player;
    playerKey0Enabled = false;
    playerKey1Enabled = false;
    playerAIEnabled = false;
    aim = NULL;

    for (Uint8 i = 0; i < CPLAYERS; i++)
    {
        // is it all? check ..


        switch( CGameState::instance()->getPlayer(i).control )
        {
        case off:
            players[i].setActive(false);
            if (aim != NULL)
            {
                aim->removePlayer((imagecolors)i);
            }
            break;
        case key0:
            player = ga->getPlayerInfo(i);

            // control only local players
            if( CGameState::instance()->getPlayer(i).node == CGameState::instance()->getNodeName() )
            {
                playerKey0 = (imagecolors)i;
                playerKey0Enabled = true;
            }

            // but need be active
            activatePlayer(player.x, player.y, (imagecolors)i);

            break;
        case key1:
            player = ga->getPlayerInfo(i);

            // control only local players
            if( CGameState::instance()->getPlayer(i).node == CGameState::instance()->getNodeName() )
            {
                playerKey1 = (imagecolors)i;
                playerKey1Enabled = true;
            }

            // but need be active
            activatePlayer(player.x, player.y, (imagecolors)i);
            break;
        case ai:
            // AI
            if ( aim == NULL )
            {
                aim = new CAIModel;
                aim->setGa(ga);
            }


            player = ga->getPlayerInfo(i);
            activatePlayer(player.x, player.y, (imagecolors)i);
            aim->addPlayer((imagecolors)i, player.x, player.y);

            playerAIEnabled = true;
            break;
        }


        //dropBomb[i] = false;
        players[i].setDropedBombs(0);

        for (Uint8 p = 0; p < CPOWERUPS; p++ )
        {
            // reset to default
            //players[i].setPowerup(p, ga->getPowerupInfo(p).bornwith );
            CGameEngine::instance()->getClient()->inventory(i, (powerup_t)p, ga->getPowerupInfo(p).bornwith);
        }
    }

    // count FPS
    cycleCount = 0;

    redraw = false;
    sdlDelay = true;
    r = nr = 0;
    // game stats
    roundHasWinner = false;
    ticksRoundHasWinner = 0;
    ticksStartGame = SDL_GetTicks();

    er.report(LOG_INFO, _("%s: CPlayState Init\n"), AT);
}

void CPlayState::Cleanup ()
{
    //SDL_FreeSurface(bg);
    delete ga;
    delete aim;

    // disable key repeat
    int resKeyRepeat = SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    if (resKeyRepeat != 0)
    {
        er.report(LOG_WARNING, _("%s: SDL_EnableKeyRepeat(%d,%d) failed.\n"), AT, SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    }

    er.report(LOG_INFO, _("%s: CPlayState Cleanup\n"), AT);
}

void CPlayState::Pause ()
{
    er.report(LOG_INFO, _("%s: CPlayState Pause\n"), AT);
}

void CPlayState::Resume ()
{
    er.report(LOG_INFO, _("%s: CPlayState Resume\n"), AT);
}

void CPlayState::HandleEvents ()
{
    if ( updatedField < GA_H * GA_W && updatedPlDefPos < CPLAYERS )
    {
        er.report(LOG_INFO, "%s: updatedField: %d, updatedPlDefPos: %d\n", AT, updatedField, updatedPlDefPos);
        return;
    }





    static bool keyDown = false;

    SDL_Event event;

    static message_t msgBomb;
    msgBomb.type = dropbomb;

    static message_t msgPlayer1, msgPlayer2;
    msgPlayer1.type = msgPlayer2.type = player;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            CGameEngine::instance()->Quit();
            break;

        case SDL_KEYDOWN:
            if (!keyDown)
            {
                keyDown = true;

                switch (event.key.keysym.sym)
                {
                case SDLK_F1:
                    drawDebug = !drawDebug;
                    break;
                case SDLK_F2:
                    sdlDelay = !sdlDelay;
                    break;
                    /*
                        case SDLK_F3:
                            ga->setFieldNum(ga->getFieldNum() + 1);
                            break;
                        case SDLK_F4:
                            ga->setFieldNum(ga->getFieldNum() - 1);
                            break;
                             */
                case SDLK_F11:
                    CGameResults::instance()->setWinner(-1); // nothing to do..
                    CGameEngine::instance()->ChangeState(CResultsState::Instance());
                    break;
/*
                case KEY0_ACTION:

                    if (players[KEY0].getActive() && players[KEY0].getLifeState() == LALIVE)
                    {
                        msgBomb.payload.mDropbomb.player = (Uint8)KEY0;
                        msgBomb.payload.mDropbomb.x = p1x;
                        msgBomb.payload.mDropbomb.y = p1y;

                        eventsQueue.push (CMessage( msgBomb, SDL_GetTicks() ));
                    }
                    break;

                case KEY1_ACTION:

                    if (players[KEY1].getActive() && players[KEY1].getLifeState() == LALIVE)
                    {
                        msgBomb.payload.mDropbomb.player = (Uint8)KEY1;
                        msgBomb.payload.mDropbomb.x = p2x;
                        msgBomb.payload.mDropbomb.y = p2y;

                        eventsQueue.push (CMessage( msgBomb, SDL_GetTicks() ));
                    }
                    break;
*/
                default:
                    break;
                }
            }
            break;

        case SDL_KEYUP:
            keyDown = false;
            break;

        default:
            break;
        }
    }

    keystates = SDL_GetKeyState(NULL);
    //SDL_PumpEvents();

    if ( playerKey0Enabled && players[playerKey0].getActive() && players[playerKey0].getLifeState() == LALIVE &&
           keystates[KEY0_ACTION] )
    {
        msgBomb.payload.mDropbomb.player = (Uint8)playerKey0;
        msgBomb.payload.mDropbomb.x = players[playerKey0].getArrayX(players[playerKey0].getArrayMiddleX());
        msgBomb.payload.mDropbomb.y = players[playerKey0].getArrayY(players[playerKey0].getArrayMiddleY());

        eventsQueue.push (CMessage( msgBomb, SDL_GetTicks() ));
    }

    if ( playerKey1Enabled && players[playerKey1].getActive() && players[playerKey1].getLifeState() == LALIVE &&
           keystates[KEY1_ACTION] )
    {
        msgBomb.payload.mDropbomb.player = (Uint8)playerKey1;
        msgBomb.payload.mDropbomb.x = players[playerKey1].getArrayX(players[playerKey1].getArrayMiddleX());
        msgBomb.payload.mDropbomb.y = players[playerKey1].getArrayY(players[playerKey1].getArrayMiddleY());

        eventsQueue.push (CMessage( msgBomb, SDL_GetTicks() ));
    }

    if ( playerKey0Enabled && players[playerKey0].getActive() && players[playerKey0].getLifeState() == LALIVE)
    {
        if( msgPlayer1.payload.mPlayer.north   != keystates[KEY0_NORTH] ||
                msgPlayer1.payload.mPlayer.south != keystates[KEY0_SOUTH] ||
                msgPlayer1.payload.mPlayer.west   != keystates[KEY0_WEST] ||
                msgPlayer1.payload.mPlayer.east != keystates[KEY0_EAST] )
        {
            msgPlayer1.payload.mPlayer.player = (Uint8)playerKey0;
            msgPlayer1.payload.mPlayer.stateRun = players[playerKey0].getRunState();
            msgPlayer1.payload.mPlayer.stateLife = players[playerKey0].getLifeState();
            msgPlayer1.payload.mPlayer.north = keystates[KEY0_NORTH];
            msgPlayer1.payload.mPlayer.south = keystates[KEY0_SOUTH];
            msgPlayer1.payload.mPlayer.west = keystates[KEY0_WEST];
            msgPlayer1.payload.mPlayer.east = keystates[KEY0_EAST];

            eventsQueue.push (CMessage( msgPlayer1, SDL_GetTicks() ));
        }
    }

    if ( playerKey1Enabled && players[playerKey1].getActive() && players[playerKey1].getLifeState() == LALIVE)
    {
        if( msgPlayer2.payload.mPlayer.north   != keystates[KEY1_NORTH] ||
                msgPlayer2.payload.mPlayer.south != keystates[KEY1_SOUTH] ||
                msgPlayer2.payload.mPlayer.west   != keystates[KEY1_WEST] ||
                msgPlayer2.payload.mPlayer.east != keystates[KEY1_EAST] )
        {
            msgPlayer2.payload.mPlayer.player = (Uint8)playerKey1;
            msgPlayer2.payload.mPlayer.stateRun = players[playerKey1].getRunState();
            msgPlayer2.payload.mPlayer.stateLife = players[playerKey1].getLifeState();
            msgPlayer2.payload.mPlayer.north = keystates[KEY1_NORTH];
            msgPlayer2.payload.mPlayer.south = keystates[KEY1_SOUTH];
            msgPlayer2.payload.mPlayer.west = keystates[KEY1_WEST];
            msgPlayer2.payload.mPlayer.east = keystates[KEY1_EAST];

            eventsQueue.push (CMessage( msgPlayer2, SDL_GetTicks() ));
        }
    }
}

void CPlayState::Update ()
{
    if (updatedField < GA_H * GA_W && updatedPlDefPos < CPLAYERS )
        return;




    Uint8 alivePlayers = 0;
    imagecolors id;
    Sint16 x, y;
    t_field fi;

    cycleCount++;

    if (aim != NULL)
        aim->doMove(&eventsQueue);

    for (Uint8 p = 0; p < CPLAYERS; p++)
    {
        if (players[p].getActive() && players[p].getLifeState() == LALIVE)
        {
            alivePlayers++;
            id = (imagecolors) p;


            bool neighborhood [4] = {false,false,false,false};
            /*
             * 0 - left
             * 1 - right
             * 2 - up
             * 3 - down
             */

            x = players[p].getArrayX(players[p].getArrayMiddleX());
            y = players[p].getArrayY(players[p].getArrayMiddleY());

            if( x - 1 < 0 )
            {
                neighborhood[0] = true;
            }
            else
            {
                fi = ga->getBoardField(x - 1, y);
                if(fi.bomb == true || fi.occupancy == TSOLID || fi.occupancy == TBRICK )
                {
                    neighborhood[0] = true;
                }
            }

            if( y - 1 < 0 )
            {
                neighborhood[2] = true;
            }
            else
            {
                fi = ga->getBoardField(x, y - 1);
                if(fi.bomb == true || fi.occupancy == TSOLID || fi.occupancy == TBRICK )
                {
                    neighborhood[2] = true;
                }
            }

            if( x + 1 >= GA_W )
            {
                neighborhood[1] = true;
            }
            else
            {
                fi = ga->getBoardField(x + 1, y);
                if(fi.bomb == true || fi.occupancy == TSOLID || fi.occupancy == TBRICK )
                {
                    neighborhood[1] = true;
                }
            }

            if( y + 1 >= GA_H )
            {
                neighborhood[3] = true;
            }
            else
            {
                fi = ga->getBoardField(x, y + 1);
                if(fi.bomb == true || fi.occupancy == TSOLID || fi.occupancy == TBRICK )
                {
                    neighborhood[3] = true;
                }
            }

            // is player blocked?
            if(neighborhood[0] && neighborhood[1] && neighborhood[2] && neighborhood[3])
            {
                if(players[p].getRunState() != XBLOCKED)
                    players[p].setRunStateSafe(XBLOCKED);
            }
            else
            {
                if(players[p].getRunState() == XBLOCKED)
                    players[p].setRunStateSafe(XSTAND);
            }
        }
    }

    if (roundHasWinner)
    { // some things freeze ...

        if ((SDL_GetTicks() - ticksRoundHasWinner) > 3000) // after 3 s show results table
        { // show results
            CGameEngine::instance()->ChangeState(CResultsState::Instance());
        }

        // bombs not exploding
        // nothing will dropped
    }
    else
    {
        if (alivePlayers == 0)
        { // no game winner
            CGameResults::instance()->setWinner(-1);
            roundHasWinner = true;
            ticksRoundHasWinner = SDL_GetTicks();
        }
        else if (alivePlayers == 1)
        { // we have winner!
            CGameResults::instance()->setWinner(id);
            roundHasWinner = true;
            ticksRoundHasWinner = SDL_GetTicks();
        }
    }

    static bool kPlayers[CPLAYERS][CDIRECTIONS];

    CMessage Msg;
    while(eventsQueue.try_pop(Msg))
    {
        printMessagePayload(Msg);

        switch(Msg.type)
        {
        case dropbomb:
            if (!roundHasWinner && players[(imagecolors)Msg.payload.mDropbomb.player].getLifeState() == LALIVE)
            {
                /*
                dropBomb(Msg.payload.mDropbomb.x, Msg.payload.mDropbomb.y,
                         (imagecolors)Msg.payload.mDropbomb.player);
                */
                CGameEngine::instance()->getClient()->bomb(
                            Msg.payload.mDropbomb.player,
                            Msg.payload.mDropbomb.x,
                            Msg.payload.mDropbomb.y,
                            0, // dont used when call dropBomb
                            bombDrop
                            );
            }

            break;
        case player:
            players[(imagecolors)Msg.payload.mPlayer.player].setLifeState( Msg.payload.mPlayer.stateLife );
            players[(imagecolors)Msg.payload.mPlayer.player].setRunStateSafe( Msg.payload.mPlayer.stateRun );

            kPlayers[(imagecolors)Msg.payload.mPlayer.player][ANORTH] = Msg.payload.mPlayer.north;
            kPlayers[(imagecolors)Msg.payload.mPlayer.player][ASOUTH] = Msg.payload.mPlayer.south;
            kPlayers[(imagecolors)Msg.payload.mPlayer.player][AWEST] = Msg.payload.mPlayer.west;
            kPlayers[(imagecolors)Msg.payload.mPlayer.player][AEAST] = Msg.payload.mPlayer.east;

            if ( !Msg.payload.mPlayer.north &&
                 !Msg.payload.mPlayer.south &&
                 !Msg.payload.mPlayer.west &&
                 !Msg.payload.mPlayer.east)
            {
                // sent the same coordinatios = stop move
                CGameEngine::instance()->getClient()->player(
                            Msg.payload.mPlayer.player,
                            players[Msg.payload.mPlayer.player].getX(),
                            players[Msg.payload.mPlayer.player].getY(),
                            players[Msg.payload.mPlayer.player].getX(),
                            players[Msg.payload.mPlayer.player].getY(),
                            (playerstate_t)Msg.payload.mPlayer.stateRun, // Uint8 to enum!
                            (playerlife_t)Msg.payload.mPlayer.stateLife // Uint8 to enum!
                        );
            }

            break;
        case gamestate:
            /*
            cout << "Game ";
            switch(m.payload.mGamestate.gamestate)
            {
            case gameStart:
                cout << "starting" << endl;
                break;
            case gameExit:
                cout << "exiting" << endl;
                break;
            };
            */
            break;
        }
    }

    // move players

    for (Uint8 p = 0; p < CPLAYERS; p++)
    {
        if (players[p].getActive() && players[p].getLifeState() == LALIVE)
        {
            movePlayer((imagecolors)p, kPlayers[p][AWEST], kPlayers[p][AEAST], kPlayers[p][ANORTH], kPlayers[p][ASOUTH] );

            x = players[p].getArrayX(players[p].getArrayMiddleX());
            y = players[p].getArrayY(players[p].getArrayMiddleY());
            fi = ga->getBoardField(x, y);

            if (fi.fire.state != -1)
            {
                playerDie((imagecolors)p, fi.fire.color);
            }

        }
    }


    if (!roundHasWinner)
    {
        // explode bombs
        for (Uint16 i = 0; i < CBOMBS; i++)
        {
            if (bombs[i].getActive())
            {
                if (bombs[i].ticksToExplode() == 0)
                { // explode!
                    //explodeBomb(i);
                    CGameEngine::instance()->getClient()->bomb(
                                0, // not used
                                0, // not used
                                0, // not used
                                i,
                                bombExplode
                                );
                }
            }
        }
    }




    for (Uint8 y = 0; y < GA_H; ++y)
    {
        for (Uint8 x = 0; x < GA_W; ++x)
        {
            fi = ga->getBoardField(x, y);
            // after 10 frames * (30 ms?) = 300
            if (fi.fire.state != -1 && fi.fire.ticksFireup > 0 && SDL_GetTicks() - fi.fire.ticksFireup > 400)
            {

                ga->setFieldValFire(x, y, -1, cgreen);
            }
        }
    }






    // fix!
    //redraw = true;

    /*
    if ((SDL_GetTicks() % 1000) == 0)
    {
    er.report(LOG_INFO, "%s: Redraw/Notredraw loop: %d/%d\n", AT, r, nr);
    }
     */
}

void CPlayState::updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup)
{
    ga->setFieldValOccupancy(x, y, occupancy);
    ga->setFieldValPowerup(x, y, powerup);

    // statistic
    updatedField++;
}

void CPlayState::setPlDefPos(Uint8 player, Uint8 x, Uint8 y)
{
    ga->setDefPlPos(player, x, y);

    // statistic
    updatedPlDefPos++;
}

void CPlayState::activatePlayer (Uint8 x, Uint8 y, imagecolors id)
{

    players[id].setType(GOT_PLAYER); // deprecated ???
    players[id].setName("KEY 0"); // deprecated

    players[id].setActive(true);
    players[id].setLifeState(LALIVE);
    players[id].setRunState(XSTAND);
    players[id].setAniDirState(SSTAND, ASOUTH);
    players[id].setColor(id);
    players[id].setArrayX(x);
    players[id].setArrayY(y);
    players[id].setGamingArea(ga);
}

void CPlayState::movePlayer (imagecolors id, bool left, bool right, bool up, bool down)
{
    Sint16 x, newX;
    Sint16 y, newY;

    // store old position
    x = newX = players[id].getX();
    y = newY = players[id].getY();

    //cout << "Player " << (int)id << " original: " << x << ":" << y << endl;

    // move with players
    if (left)
    {
        if (down || up)
        {
            if (down)
            {
                players[id].movePlayerRel(AWEST, ASOUTH);
            }
            else
            {
                players[id].movePlayerRel(AWEST, ANORTH);
            }
        }
        else
        {
            players[id].movePlayerRel(AWEST, ANONE);
        }
    }
    else
    {
        if (down)
        {
            if (left || right)
            {
                if (left)
                {
                    players[id].movePlayerRel(ASOUTH, AWEST);
                }
                else
                {
                    players[id].movePlayerRel(ASOUTH, AEAST);
                }
            }
            else
            {
                players[id].movePlayerRel(ASOUTH, ANONE);
            }
        }
        else
        {
            if (right)
            {
                if (down || up)
                {
                    if (down)
                    {
                        players[id].movePlayerRel(AEAST, ASOUTH);
                    }
                    else
                    {
                        players[id].movePlayerRel(AEAST, ANORTH);
                    }
                }
                else
                {
                    players[id].movePlayerRel(AEAST, ANONE);
                }
            }
            else
            {
                if (up)
                {
                    if (left || right)
                    {
                        if (left)
                        {
                            players[id].movePlayerRel(ANORTH, AWEST);
                        }
                        else
                        {
                            players[id].movePlayerRel(ANORTH, AEAST);
                        }
                    }
                    else
                    {
                        players[id].movePlayerRel(ANORTH, ANONE);
                    }
                }
                else
                {
                    if (players[id].getRunState() == XWALK)
                    {

                        players[id].setRunStateSafe(XSTAND);
                        redraw = true;
                    }
                }
            }
        }
    } // end of move players

    // store new position
    newX = players[id].getX();
    newY = players[id].getY();

    if (x != newX || y != newY )
    {
        /*
        cout << "Player " << (int)id
             << " old: " << x << ":" << y
             << " new: " << newX << ":" << newY
             << endl;
             */

        // restore position old ...
        players[id].setX(x);
        players[id].setY(y);

        // ... and sent new throught network
        CGameEngine::instance()->getClient()->player((Uint8)id, x, y, newX, newY, (playerstate_t)players[id].getRunState(), (playerlife_t)players[id].getLifeState());
    }
}

void CPlayState::dropBomb(Sint16 x, Sint16 y, imagecolors color)
{
    if (ga->getBoardField(x,y).bomb == false && ga->getBoardField(x,y).occupancy == TBLANK)
    {
        if (players[color].dropBomb()) // have we bomb?
        {
            for (Uint16 i = 0; i < CBOMBS; i++)
            {
                if (!bombs[i].getActive()) // get free bomb slot
                {
                    bombs[i].setActive(true);
                    bombs[i].setColor(color);
                    bombs[i].setFlameLength(players[color].getPowerup(PFLAME));

                    bombs[i].moveAbs(0, 0); // because setRELx() above. more: see setRelX
                    bombs[i].setRelX(x * BOX_W);
                    bombs[i].setRelY(y * BOX_H);

                    bombs[i].startAnim();
                    ga->setFieldValBomb(x, y, true);
                    break;
                }
            }
        }
    }
}

void CPlayState::playersDie (Sint16 x, Sint16 y, imagecolors color)
{
    //er.report(LOG_INFO, "%s: playersDie(%d, %d)\n", AT, x, y);
    for (Uint8 p = 0; p < CPLAYERS; p++)
    {
        if (players[p].getActive() && players[p].getLifeState() == LALIVE)
        {
            if (x == players[p].getArrayX(players[p].getArrayMiddleX())
                && y == players[p].getArrayY(players[p].getArrayMiddleY()))
            {

                playerDie((imagecolors) p, color);
            }
        }
    }
}

void CPlayState::playerDie (imagecolors victim, imagecolors killer)
{
    assert(victim < CPLAYERS);
    // active?
    players[victim].setLifeState(LDEAD);
    players[victim].setAniDirState(SDIE, ANONE);
    // remove AI
    if (aim != NULL)
        aim->removePlayer(victim);

    if (victim == killer) // kill yourself
    {
        CGameResults::instance()->UpdateKills(victim, -1);
    }
    else
    {

        CGameResults::instance()->UpdateKills(killer, 1);
    }

    // play sound :-) uuuuuuuaaaaaaaaa
    CPlaySound::instance()->PlayDie();
}

void CPlayState::explodeBomb (Uint8 x, Uint8 y)
{
    for (Uint16 i = 0; i < CBOMBS; i++)
    {
        if (bombs[i].getActive() && bombs[i].getArrayX() == x && bombs[i].getArrayY() == y)
        {
            //explodeBomb(i);
            CGameEngine::instance()->getClient()->bomb(
                        0, // not used
                        0, // not used
                        0, // not used
                        i,
                        bombExplode
                        );
        }
    }
}

void CPlayState::explodeBomb (Uint16 i)
{
    Uint8 flameLen = bombs[i].getFlameLength();
    Sint16 bombX, bombY;

    bombX = bombs[i].getArrayX();
    bombY = bombs[i].getArrayY();

    bombs[i].setActive(false);
    ga->setFieldValBomb(bombX, bombY, false);
    CPlaySound::instance()->PlayExplode();

    // middle
    playersDie(bombX, bombY, bombs[i].getColor());
    ga->setFieldValFire(bombX, bombY, FCENTER, bombs[i].getColor());

    // around
    Uint8 newX = bombX, newY = bombY;
    bool r[CDIRECTIONS] = {true, true, true, true};
    for (Uint8 e = 1; e <= flameLen; e++)
    {
        //<<<
        if (r[AWEST] && bombX - e >= 0)
        {
            newX = bombX - e;
            if ((r[AWEST] = explodeField(newX, bombY)) == true)
            {
                playersDie(newX, bombY, bombs[i].getColor());

                if (ga->getBoardField(newX, bombY).fire.state == -1)
                { // dont overwrite flames
                    if (e == flameLen)
                    {
                        ga->setFieldValFire(newX, bombY, FTIPWEST, bombs[i].getColor());
                    }
                    else
                    {
                        ga->setFieldValFire(newX, bombY, FMIDWEST, bombs[i].getColor());
                    }
                }
            }
        }
        //>>>
        if (r[AEAST] && bombX + e < GA_W)
        {
            newX = bombX + e;
            if ((r[AEAST] = explodeField(newX, bombY)) == true)
            {
                playersDie(newX, bombY, bombs[i].getColor());

                if (ga->getBoardField(newX, bombY).fire.state == -1)
                {// dont overwrite flames
                    if (e == flameLen)
                    {
                        ga->setFieldValFire(newX, bombY, FTIPEAST, bombs[i].getColor());
                    }
                    else
                    {
                        ga->setFieldValFire(newX, bombY, FMIDEAST, bombs[i].getColor());
                    }
                }
            }
        }
        //^^^
        if (r[ANORTH] && bombY - e >= 0)
        {
            newY = bombY - e;
            if ((r[ANORTH] = explodeField(bombX, newY)) == true)
            {
                playersDie(bombX, newY, bombs[i].getColor());

                //if (ga->getBoardField(bombX, newY).fire.state == -1)
                {// dont overwrite flames
                    if (e == flameLen)
                    {
                        ga->setFieldValFire(bombX, newY, FTIPNORTH, bombs[i].getColor());
                    }
                    else
                    {
                        ga->setFieldValFire(bombX, newY, FMIDNORTH, bombs[i].getColor());
                    }
                }
            }
        }
        //vvv
        if (r[ASOUTH] && bombY + e < GA_H)
        {
            newY = bombY + e;
            if ((r[ASOUTH] = explodeField(bombX, newY)) == true)
            {
                playersDie(bombX, newY, bombs[i].getColor());

                if (ga->getBoardField(bombX, newY).fire.state == -1)
                {// dont overwrite flames
                    if (e == flameLen)
                    {
                        ga->setFieldValFire(bombX, newY, FTIPSOUTH, bombs[i].getColor());
                    }
                    else
                    {

                        ga->setFieldValFire(bombX, newY, FMIDSOUTH, bombs[i].getColor());
                    }
                }
            }
        }
    }

    if ( players[bombs[i].getColor() ].getDropedBombs() > 0 )
    {
        players[ bombs[i].getColor() ].setDropedBombs(players[bombs[i].getColor() ].getDropedBombs() - 1);
    }
    else
    {
        er.report(LOG_WARNING, _("%s: Player %d has 0 droped bombs. Error?\n"), AT, bombs[i].getColor());
    }
}

/**
 * Explode field. When exist bomb, explode!
 * @param x
 * @param y
 * @return Return false, when fire up PowerUp, Brick, Bomb or Stone. True (= continue) otherwise.
 */
bool CPlayState::explodeField (Uint8 x, Uint8 y)
{
    //er.report(LOG_INFO, "%s: explodeField(%d,%d)\n", AT, x, y);
    t_field f = ga->getBoardField(x, y);

    if (f.bomb)
    {
        er.report(LOG_INFO, "%s: explodeBomb(%d,%d)\n", AT, x, y);
        explodeBomb(x, y);
        return false;
    }

    if (f.powerup != -1) // powerups? fireup and stop
    {
        ga->setFieldValOccupancy(x, y, TBLANK);
        return false;
    }

    if (f.occupancy == TSOLID) // stone? stop.
    {
        return false;
    }

    if (f.occupancy == TBRICK) // brick? fireup and stop.
    {

        // TODO: send throught network
        ga->setFieldValOccupancy(x, y, TBLANK);
        ga->setFieldValFire(x, y, FBRICK, cgreen);
        /**
         *  TODO: possible generate new powerups here.....
         */
        Sint32 r = rand();

        if ( r < RAND_MAX / 2 )
        { // yes, generate
            if ( (r % 2) == 0 )
            {
                ga->setFieldValPowerup(x, y, PBOMB);
            }
            else
            {
                ga->setFieldValPowerup(x, y, PFLAME);
            }
        }
        /**/
        return false;
    }

    return true;
}

double CPlayState::calculateCPS()
{
    Uint32 currentTime = (SDL_GetTicks() - ticksStartGame);
    static Uint32 previousTime = currentTime;
    static double cps = 0.0f;

    Uint32 timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        cps = cycleCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        cycleCount = 0;
    }

    return cps;
}

double CPlayState::calculateFPS()
{
    Uint32 currentTime = (SDL_GetTicks() - ticksStartGame);
    static Uint32 previousTime = currentTime;
    static double fps = 0.0f;

    Uint32 timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }

    return fps;
}

void CPlayState::Draw ()
{
    frameCount++;

    DrawBackground();
    DrawSolidObjects();
    //DrawNonSolidObjects(); // nothing in.. maybe in future :)
    DrawBombsFlames();
    DrawPlayers();
    if (drawDebug)
        DrawDebug();

    redraw = true; //// FIX!

    if (redraw)
    {
        r++;
        // Update the screen
        if ((SDL_GetVideoSurface()->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF)
        {
            SDL_Flip(SDL_GetVideoSurface());
        }
        else
        {
            SDL_UpdateRect(SDL_GetVideoSurface(), 0, 0, 0, 0);
        }
    }
    else
    {
        nr++;
    }

    if (sdlDelay)
        SDL_Delay(1);

    redraw = false;
}

void CPlayState::DrawBackground ()
{
    // background

    CStringDraw::instance()->drawObject(b_field[ga->getFieldNum()], 0, 0, false);
}

void CPlayState::DrawSolidObjects ()
{
    const int MAX_LEN = 300;
    char buffer [MAX_LEN] = "";

    // score results
    Uint16 offsetX = 10;
    for (Uint8 i = 0; i < CPLAYERS; i += 2)
    {
        snprintf(buffer, MAX_LEN, _("S:%d K:%d"), CGameResults::instance()->getScore(i), CGameResults::instance()->getKills(i));
        CStringDraw::instance()->drawString(offsetX, 9, buffer, (imagecolors) i);
        snprintf(buffer, MAX_LEN, _("S:%d K:%d"), CGameResults::instance()->getScore(i + 1), CGameResults::instance()->getKills(i + 1));
        CStringDraw::instance()->drawString(offsetX, 25, buffer, (imagecolors) (i + 1));

        offsetX += 100;
    }

    // object on the map
    t_field fi;
    Sint32 frame = 0;
    for (Uint8 y = 0; y < GA_H; ++y)
    {
        for (Uint8 x = 0; x < GA_W; ++x)
        {
            fi = ga->getBoardField(x, y);

            if (ga->getFieldNum() == 4 && fi.warp != NULL)
            {
                frame = warps.getFrame();
                if (warps.getNewFrame(frame) != -1)
                {
                    redraw = true;
                }

                CStringDraw::instance()->drawObject(
                                                    warps.getFrameSurface(frame, cgreen),
                                                    warps.getRelX() + x * BOX_W,
                                                    warps.getRelY() + y * BOX_H,
                                                    false
                                                    );

                continue; // dira musi byt videt

            }

            if (fi.occupancy != TBLANK)
            {
                frame = tiles[ga->getFieldNum()][fi.occupancy].getFrame();
                if (tiles[ga->getFieldNum()][fi.occupancy].getNewFrame(frame) != -1)
                {
                    redraw = true;
                }

                CStringDraw::instance()->drawObject(
                                                    tiles[ga->getFieldNum()][fi.occupancy].getFrameSurface(frame, cgreen),
                                                    tiles[ga->getFieldNum()][fi.occupancy].getRelX() + x * BOX_W,
                                                    tiles[ga->getFieldNum()][fi.occupancy].getRelY() + y * BOX_H,
                                                    false
                                                    );

            }
            else
            {
                if ((ga->getFieldNum() == 2 || ga->getFieldNum() == 3) && fi.arrow != NULL)
                { /* v originalnim bombicovi jsou sipky divne umistene --> bud je chyba
                     v propoctu, nebo to taky nejak fixluji :) */
                    frame = arrows[ga->getBoardField(x, y).arrow->direction].getFrame();
                    if (arrows[ga->getBoardField(x, y).arrow->direction].getNewFrame(frame) != -1)
                    {
                        redraw = true;
                    }

                    CStringDraw::instance()->drawObject(
                                                        arrows[ga->getBoardField(x, y).arrow->direction].getFrameSurface(frame, cgreen),
                                                        arrows[ga->getBoardField(x, y).arrow->direction].getRelX() + x * BOX_W + BOX_H2 / 2,
                                                        arrows[ga->getBoardField(x, y).arrow->direction].getRelY() + y * BOX_H + BOX_H2,
                                                        false
                                                        );

                }

                if (ga->getFieldNum() == 10 && fi.conveyor != NULL)
                {
                    frame = conveyors[ga->getBoardField(x, y).conveyor->direction].getFrame();
                    if (conveyors[ga->getBoardField(x, y).conveyor->direction].getNewFrame(frame) != -1)
                    {
                        redraw = true;
                    }

                    CStringDraw::instance()->drawObject(
                                                        conveyors[ga->getBoardField(x, y).conveyor->direction].getFrameSurface(frame, cgreen),
                                                        conveyors[ga->getBoardField(x, y).conveyor->direction].getRelX() + x * BOX_W,
                                                        conveyors[ga->getBoardField(x, y).conveyor->direction].getRelY() + y * BOX_H,
                                                        false
                                                        );

                }

                if (ga->getFieldNum() == 9 && fi.trampoline != NULL)
                {
                    frame = trampolines.getFrame();
                    if (trampolines.getNewFrame(frame) != -1)
                    {
                        redraw = true;
                    }

                    CStringDraw::instance()->drawObject(
                                                        trampolines.getFrameSurface(frame, cgreen),
                                                        trampolines.getRelX() + x * BOX_W,
                                                        trampolines.getRelY() + y * BOX_H + BOX_H2,
                                                        false
                                                        );

                }

                if (fi.powerup != -1)
                {
                    frame = powerups[fi.powerup].getFrame();
                    if (powerups[fi.powerup].getNewFrame(frame) != -1)
                    {

                        redraw = true;
                    }

                    CStringDraw::instance()->drawObject(
                                                        powerups[fi.powerup].getFrameSurface(frame, cgreen),
                                                        powerups[fi.powerup].getRelX() + x * BOX_W,
                                                        powerups[fi.powerup].getRelY() + y * BOX_H,
                                                        false
                                                        );


                }
            }
        }
    }
}

void CPlayState::DrawNonSolidObjects ()
{

}

void CPlayState::DrawBombsFlames ()
{
    Sint32 frame = 0;

    for (Uint8 i = 0; i < CBOMBS; i++)
    {
        if (bombs[i].getActive())
        {
            frame = bombs[i].getFrame();
            if (bombs[i].getNewFrame(frame) != -1)
            {
                redraw = true;
            }

            CStringDraw::instance()->drawObject(
                                                bombs[i].getFrameSurface(frame, bombs[i].getColor()),
                                                bombs[i].getSurfaceX(frame) + bombs[i].getRelX(),
                                                bombs[i].getSurfaceY(frame) + bombs[i].getRelY(),
                                                false
                                                );

        }
    }

    t_field fi;
    for (Uint8 y = 0; y < GA_H; ++y)
    {
        for (Uint8 x = 0; x < GA_W; ++x)
        {
            fi = ga->getBoardField(x, y);
            if (fi.fire.state != -1 && fi.fire.ticksFireup > 0)
            {
                if (fi.fire.state == FBRICK)
                {
                    Uint32 num = ga->getFieldNum();
                    frame = firesb[num].getFrame();
                    if (firesb[num].getNewFrame(frame) != -1)
                    {
                        redraw = true;
                    }

                    if (firesb[num].getFrameSurface(frame, cgreen) != NULL)
                    {

                        CStringDraw::instance()->drawObject(
                                                            firesb[num].getFrameSurface(frame, cgreen),
                                                            firesb[num].getRelX(x * BOX_W),
                                                            firesb[num].getRelY(y * BOX_H),
                                                            false
                                                            );
                    }
                }
                else
                {
                    frame = fires[fi.fire.state].getFrame();
                    if (fires[fi.fire.state].getNewFrame(frame) != -1)
                    {
                        redraw = true;
                    }

                    /*
                    er.report(LOG_INFO, "%s: fire at [%d,%d]: %d %d--%d %d %d draw at [%d,%d]\n",
                    AT, x, y, fi.fire.state, fi.fire.ticksFireup, fires[fi.fire.state].getTicksDoneAnim(), fi.fire.color, frame,
                    x * BOX_W,
                    y * BOX_H
                    );
                     */

                    //fires[fi.fire.state].getRelX() * BOX_W;
                    //fires[fi.fire.state].getRelY() * BOX_H;

                    if (fires[fi.fire.state].getFrameSurface(frame, fi.fire.color) != NULL)
                    {

                        CStringDraw::instance()->drawObject(
                                                            fires[fi.fire.state].getFrameSurface(frame, fi.fire.color),
                                                            fires[fi.fire.state].getRelX(x * BOX_W) + fires[fi.fire.state].getSurfaceX(frame),
                                                            fires[fi.fire.state].getRelY(y * BOX_H) + fires[fi.fire.state].getSurfaceY(frame),
                                                            false
                                                            );
                    }
                }
            }
        }
    }
}

void CPlayState::DrawPlayers ()
{
    Uint8 AniState = 0;
    Sint32 frame = 0;
    /* mame 10 barev */
    for (Uint8 i = 0; i < CPLAYERS; i++)
    {
        if (players[i].getActive())
        {
            AniState = players[i].getAniState();

            frame = players[i].getFrame();


            if (players[i].getNewFrame(frame) != -1)
            {
                redraw = true;
            }


            //redraw = true;


            if (AniState != SDIE && AniState != SSPIN)
            {
                CStringDraw::instance()->drawObject(
                                                    players[i].getShadow(),
                                                    players[i].getSurShadowX() + players[i].getRelX(),
                                                    players[i].getSurShadowY() + players[i].getRelY(),
                                                    false
                                                    );
            }


            if (players[i].getLifeState() == LDEAD && players[i].getAnimDone() && (SDL_GetTicks() - players[i].getTicksDoneAnim() > 200))
            {
                // nothing?
            }
            else
            {

                CStringDraw::instance()->drawObject(
                                                    players[i].getFrameSurface(frame, players[i].getColor()),
                                                    players[i].getSurfaceX(frame) + players[i].getRelX(),
                                                    players[i].getSurfaceY(frame) + players[i].getRelY(),
                                                    false
                                                    );
            }


        }
    }
}

void CPlayState::DrawDebug ()
{
    const int MAX_LEN = 300;
    char buffer [MAX_LEN] = "";

    const int offsetX = 25;
    int offsetY = 57;

    int index = 1;

    snprintf(buffer, MAX_LEN, "SDL_GetTicks(): %d", SDL_GetTicks());
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "ticksStartGame: %d (%d s)", ticksStartGame, (SDL_GetTicks() - ticksStartGame) / 1000);
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "getGameWinner(): %d", CGameResults::instance()->getGameWinner());
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "roundHasWinner: %d", roundHasWinner);
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "ticksWhenRoundHasWinner: %d (%d)", ticksRoundHasWinner, SDL_GetTicks() - ticksRoundHasWinner);
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "SDL_Delay() in Draw() called: %d", sdlDelay);
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);
    snprintf(buffer, MAX_LEN, "CPS: %.2f, FPS: %.2f", calculateCPS(), calculateFPS() );
    CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);

    snprintf(buffer, MAX_LEN, "%s (KEY 0) %d:%d %d:%d B%d F%d S%d K%d",
             CGameState::instance()->color2str(players[playerKey0].getColor()),
             players[playerKey0].getArrayX(players[playerKey0].getArrayMiddleX()),
             players[playerKey0].getArrayY(players[playerKey0].getArrayMiddleY()),
             players[playerKey0].getArrayX(),
             players[playerKey0].getArrayY(),
             players[playerKey0].getPowerup(PBOMB),
             players[playerKey0].getPowerup(PFLAME),
             players[playerKey0].getPowerup(PSKATE),
             players[playerKey0].getPowerup(PKICKER));
    if( playerKey0Enabled )
        CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);

    snprintf(buffer, MAX_LEN, "%s (KEY 1) %d:%d %d:%d B%d F%d S%d K%d",
             CGameState::instance()->color2str(players[playerKey1].getColor()),
             players[playerKey1].getArrayX(players[playerKey1].getArrayMiddleX()),
             players[playerKey1].getArrayY(players[playerKey1].getArrayMiddleY()),
             players[playerKey1].getArrayX(),
             players[playerKey1].getArrayY(),
             players[playerKey1].getPowerup(PBOMB),
             players[playerKey1].getPowerup(PFLAME),
             players[playerKey1].getPowerup(PSKATE),
             players[playerKey1].getPowerup(PKICKER));
    if( playerKey1Enabled )
        CStringDraw::instance()->drawString(offsetX, offsetY + 20 * index++, buffer);



    for (Uint16 i = 0; i < CBOMBS; i++)
    {
        if (bombs[i].getActive())
        {
            snprintf(buffer, MAX_LEN, "Bomb[%d] .array[X,Y]=%d,%d .flame=%d .color=%d .toExplode=%d",
                     i, bombs[i].getArrayX(), bombs[i].getArrayY(),
                     bombs[i].getFlameLength(), bombs[i].getColor(), bombs[i].ticksToExplode());
            CStringDraw::instance()->drawString(offsetX, offsetY + 20 * (index + i), buffer);
        }
    }

    redraw = true;
}

void CPlayState::printMessagePayload( CMessage &m )
{
    cout << "[" << SDL_GetTicks() << "] Time: " << m.timestamp << " Payload: ";
    switch(m.type)
    {
    case dropbomb:
        cout << "Player " << (Uint32)m.payload.mDropbomb.player
             << " dropped bomb at "
             << (Uint32)m.payload.mDropbomb.x
             << "," << (Uint32)m.payload.mDropbomb.y
             << endl;
        break;
    case player:
        cout << "Player " << (Uint32)m.payload.mPlayer.player
             << " run: " << (Uint32)m.payload.mPlayer.stateRun
             << " life: " << (Uint32)m.payload.mPlayer.stateLife
             << " u" << m.payload.mPlayer.north
             << " d" << m.payload.mPlayer.south
             << " l" << m.payload.mPlayer.west
             << " r" << m.payload.mPlayer.east
             << endl;
        break;
    default:
        cout << "Type: " << m.type << endl;
        break;
        /*
    case gamestate:
        cout << "Game ";
        switch(m.payload.mGamestate.gamestate)
        {
        case gameStart:
            cout << "starting" << endl;
            break;
        case gameExit:
            cout << "exiting" << endl;
            break;
        default:

        };
        break;
        */
    };
}
