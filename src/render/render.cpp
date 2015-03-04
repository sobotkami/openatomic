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

#include <render/render.h>

#include <core/go_bomb.h>
#include <core/go_player.h>

extern int MUTEX_INIT(MUTEX *mutex);
extern int MUTEX_LOCK(MUTEX *mutex);
extern int MUTEX_UNLOCK(MUTEX *mutex);

extern CGO_Player players[CCOLORS];
extern CGO_Bomb   bombs[CBOMBS];
extern CGameObject tiles[CFIELDS][CTILES];
extern CGameObject arrows[CDIRECTIONS];
extern CGameObject trampolines;
extern CGameObject warps;
extern CGameObject conveyors[CDIRECTIONS];
extern CGameObject powerups[CPOWERUPSR];

using namespace std;

Uint8 CRender::init ( bool useOpenGL, CCfgReader * cfg )
{
    const SDL_VideoInfo * info;
    CPcxReader pcxreader;
    ostringstream stm;
    Uint16 i = 0;
    Uint16 cpcxs = 0;
    SDL_version sdlVersion;

    this->cfg = cfg;


    SDL_VERSION(&sdlVersion);

    er.report( LOG_INFO, _("%s: SDL version is %d.%d.%d\n"), AT, (Uint16)sdlVersion.major, (Uint16)sdlVersion.minor, (Uint16)sdlVersion.patch );

    if ( SDL_Init ( SDL_SUBSYSTEMS ) == -1 )
    {
        er.report( LOG_ERROR, _("%s: Unable to initialize SDL: %s!\n"), AT, SDL_GetError() );
        return 1;
    }

    info = SDL_GetVideoInfo();
    if ( !info )
    {
        er.report( LOG_ERROR, _("%s: SDL is unable to get the video info: %s!\n"), AT, SDL_GetError() );
        return 1;
    }

//     if ( useOpenGL )
//     {
//         // Initializing SDL attributes with OpenGL
// //         SDL_GL_SetAttribute ( SDL_GL_BUFFER_SIZE, WIN_BPP );
// //         SDL_GL_SetAttribute ( SDL_GL_DEPTH_SIZE, WIN_BPP );
// //         SDL_GL_SetAttribute ( SDL_GL_DOUBLEBUFFER, 1 );
// //         SDL_GL_SetAttribute ( SDL_GL_STENCIL_SIZE, 0 );
// //         SDL_GL_SetAttribute ( SDL_GL_ACCUM_RED_SIZE, 0 );
// //         SDL_GL_SetAttribute ( SDL_GL_ACCUM_GREEN_SIZE, 0 );
// //         SDL_GL_SetAttribute ( SDL_GL_ACCUM_BLUE_SIZE, 0 );
// //         SDL_GL_SetAttribute ( SDL_GL_ACCUM_ALPHA_SIZE, 0 );
//         SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
//         SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
//         SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
//         SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
//         SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
//         this->useOpenGl = useOpenGL;
//         win_flags |= SDL_OPENGL;
//     }

    if ( info->hw_available )
    {
        er.report( LOG_INFO, _("%s: HW surface and back buffer are available.\n"), AT );
        win_flags |= SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF;
    }
    else
    {
        er.report( LOG_WARNING, _("%s: HW surface and back buffer are NOT available!\n"), AT );
    }

    if ( info->blit_hw )
    {
        er.report( LOG_INFO, _("%s: HW acceleration blitting is available!\n"), AT );
        win_flags |= SDL_HWACCEL;
    }
    else
    {
        er.report( LOG_WARNING, _("%s: HW acceleration blitting is NOT available!\n"), AT );
    }

    if ( cfg->getFullscreen() )
        win_flags |= SDL_FULLSCREEN;

    screen = SDL_SetVideoMode ( WIN_WIDTH, WIN_HEIGHT, WIN_BPP, win_flags );
    if ( screen == NULL )
    {
        er.report( LOG_ERROR, _("%s: Unable to set %dx%d video resolution: %s!\n"), AT, WIN_WIDTH, WIN_HEIGHT, SDL_GetError() );
        return 1;
    }

//     if (useOpenGL)
//     {
//         glClearColor(0.0, 0.0, 0.0, 0.0);
//         glClearDepth(1.0);
//         glDepthFunc(GL_LEQUAL);
//         glEnable(GL_DEPTH_TEST);
//         glShadeModel(GL_SMOOTH);
//         glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//     }

    SDL_WM_SetCaption ( WIN_TITLE, NULL );

    // Nastaveni filtru udalosti
    EnableEventState(SDL_MOUSEMOTION, SDL_IGNORE);
//     EnableEventState(SDL_KEYUP, SDL_IGNORE);
    EnableEventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    EnableEventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EnableKeyRepeat (1, KEYREPEAT);

    /* inicializace fontu */
    font.read( "FONT6.FON" );

#if defined(CONF_PF_WINDOWS)
    string os = "Windows";
#elif defined(CONF_PF_LINUX)
    string os = "Linux";
#elif defined(CONF_PT_MACOSX)
    string os = "Mac OS X";
#else
    string os = "Other OS";
#endif

    string version = "555";//CONF_VERSION;

    string topString = version + _(" for ") + os;
    Uint16 topStringSize = font.getStringWidth(topString);
    drawString( getDimensionsOfScreen().w / 2 - topStringSize / 2, 10, topString, cyellow, true );

    /* nacteni pozadi */
    cpcxs = 0;
    drawLoadString("DATA/RES/DRAW.PCX", cpcxs++, 33 );
    b_drawgame = pcxreader.read ( "DATA/RES/DRAW.PCX" );
    
    drawLoadString("DATA/RES/ROULETTE.PCX", cpcxs++, 33 );
    b_bonusgame = pcxreader.read ( "DATA/RES/ROULETTE.PCX" );

    drawLoadString("DATA/RES/RESULTS.PCX", cpcxs++, 33 );
    b_results = pcxreader.read ( "DATA/RES/RESULTS.PCX" );

    drawLoadString("DATA/RES/MAINMENU.PCX", cpcxs++, 33 );
    b_mainmenu = pcxreader.read ( "DATA/RES/MAINMENU.PCX" );

    drawLoadString("DATA/RES/TEAM0.PCX", cpcxs++, 33 );
    b_team[0] = pcxreader.read ( "DATA/RES/TEAM0.PCX" );
    
    drawLoadString("DATA/RES/TEAM1.PCX", cpcxs++, 33 );
    b_team[1] = pcxreader.read ( "DATA/RES/TEAM1.PCX" );
    
    for ( i = 0; i < CCOLORS; ++i )
    {
        stm << "DATA/RES/VICTORY" << i << ".PCX";
        drawLoadString( stm.str(), cpcxs++, 33  );
        b_victory[i] = pcxreader.read ( stm.str() );
        stm.str("");
    }
    for ( i = 0; i < CBACKGROUNDS; ++i )
    {
        stm << "DATA/RES/GLUE" << i << ".PCX";
        drawLoadString( stm.str(), cpcxs++, 33  );
        b_glue[i] = pcxreader.read ( stm.str() );
        stm.str("");
    }
    /*
    for ( i = 0; i < CFIELDS; ++i )
    {
        stm << "DATA/RES/FIELD" << i << ".PCX";
        drawLoadString( stm.str(), cpcxs++, 33  );
        b_field[i] = pcxreader.read ( stm.str() );
        stm.str("");
    }
     */

    /* vychozi pozadi */
    this->background = b_mainmenu;

    /* pozadi ve hre */
    if ( cfg->getFieldNum() == -1 )
    {
        srand ( time(NULL) );
        this->ga.setFieldNum( rand() % CFIELDS );
    }
    else
    {
        this->ga.setFieldNum( cfg->getFieldNum() );
    }

    if ( ga.getFieldNum() == 10 )
    {
        for ( Uint8 i = 0; i < CDIRECTIONS; ++i )
        {
            conveyors[i].startAnim();
        }
    }
    else
    {
        for ( Uint8 i = 0; i < CDIRECTIONS; ++i )
        {
            conveyors[i].stopAnim();
        }
    }

    stm << "DATA/SCHEMES/" << cfg->getScheme();
    drawLoadString(stm.str());

    if ( ga.loadScheme( stm.str() ) == false )
    {
        er.report( LOG_ERROR, _("%s: Unable to open file '%s'!\n"), AT, stm.str().c_str() );
    }
    stm.str("");

    /* 30 ms = 1000 / 30 = 33 FPS strop ... */
    tRedraw = SDL_AddTimer ( 30, timerCallbackRedraw, &(this->timerOperationRedrawCounter) );
    if ( tRedraw == NULL )
    {
        er.report( LOG_ERROR, _("%s: Failed to initialize the timer interrupt!\n"), AT );
    }
    else
    {
        er.report( LOG_INFO, _("%s: Successfuly initialized the timer interrupt.\n"), AT );
    }

    return 0;
}

Uint8 CRender::ChangeEventState( Uint8 type )
{
    // Dotaz na aktualni stav
    if (SDL_EventState(type, SDL_QUERY) == SDL_ENABLE)
    {
        // Zrusi vkladani
        return SDL_EventState(type, SDL_IGNORE);
    }
    else
    {
        // Povoli vkladani
        return SDL_EventState(type, SDL_ENABLE);
    }
}

/*
 * Povoli/zrusi vkladani udalosti do fronty na zaklade parametru
 */

Uint8 CRender::EnableEventState( Uint8 type, int state )
{
    return SDL_EventState( type, state );
}

void CRender::PushQuitEvent()
{
    SDL_Event event;    // Vytvoreni udalosti
    event.type = SDL_QUIT;  // Nastaveni parametru
    SDL_PushEvent(&event);  // Vlozeni do fronty
}

Uint32 timerCallbackRedraw ( Uint32 interval, void* param )
{
    return ( ( CRender * ) param )->timerOperationRedraw ( interval, param );
}

Uint32 CRender::timerOperationRedraw ( Uint32 interval, void* param )
{
    SDL_Event event;    // Vytvoreni udalosti
    event.type = REDRAW;    // Nastaveni parametru
// //     bool redraw = false;
    Uint32 c = 0;

    c = *(Uint32 *)param;

    if ( c > 0 )
    {
        *(Uint32 *)param = 0;
        SDL_PushEvent(&event);
    }

// // //     cout << "X" << flush;
// // 
// // //     SDL_PushEvent(&event);
// // //     return interval;
// // 
// //     MUTEX_LOCK(&mutRedraw);
// // //     pthread_mutex_lock ( &mutRedraw );
// // //     if ( SDL_LockMutex ( mutRedraw ) < 0 )
// // //     {
// // //         cerr << SDL_GetError() << endl << flush;
// // //         exit(1);
// // //     }
// // 
// //     if ( *(Uint32 *)param > 0 )
// //     {
// //         c = *(Uint32 *)param;
// //         *(Uint32 *)param = 0;
// //         redraw = true;
// //     }
// // 
// //     MUTEX_UNLOCK(&mutRedraw);
// // //     pthread_mutex_unlock ( &mutRedraw );
// // //     if ( SDL_UnlockMutex ( mutRedraw ) < 0 )
// // //     {
// // //         cerr << SDL_GetError() << endl << flush;
// // //         exit(1);
// // //     }
// // 
// //     if ( redraw )
// //     {
// //         SDL_PushEvent(&event);  // Vlozeni do fronty
// // //         cout << "Pocet zadosti: " << c << endl << flush;
// //     }

    return interval;
}

void CRender::PushRedrawEvent()
{
    SDL_Event event;    // Vytvoreni udalosti
    event.type = REDRAW;    // Nastaveni parametru
// //     bool redraw = false;
    Uint32 c = 0;

    timerOperationRedrawCounter++;
    if ( timerOperationRedrawCounter > 200 )
    {
        c = timerOperationRedrawCounter;
        timerOperationRedrawCounter = 0;
        SDL_PushEvent(&event);
        er.report( LOG_INFO, _("%s: Exceeded %d requests to redraw scene - drawing immediately.\n"), AT, c );
    }

// // //     cout << "PushRedrawEvent " << flush;
// // 
// // //     SDL_PushEvent(&event);
// // //     return ;
// // 
// //     MUTEX_LOCK(&mutRedraw);
// // //     pthread_mutex_lock ( &mutRedraw );
// // //     if ( SDL_LockMutex ( mutRedraw ) < 0 )
// // //     {
// // //         cerr << SDL_GetError() << endl << flush;
// // //         exit(1);
// // //     }
// // 
// //     timerOperationRedrawCounter++;
// // 
// //     /* kolik objektu vykreslime max v jednom cyklu?? 150 pri 100 bombach, conveyors a dvou pohyb. hracich .. malo? */
// //     if ( timerOperationRedrawCounter > 200 )
// //     {
// //         c = timerOperationRedrawCounter;
// //         timerOperationRedrawCounter = 0;
// //         redraw = true;
// //     }
// // 
// //     MUTEX_UNLOCK(&mutRedraw);
// // //     pthread_mutex_unlock ( &mutRedraw );
// // //     if ( SDL_UnlockMutex ( mutRedraw ) < 0 )
// // //     {
// // //         cerr << SDL_GetError() << endl << flush;
// // //         exit(1);
// // //     }
// // 
// //     if ( redraw )
// //     {
// //         SDL_PushEvent(&event);  // Vlozeni do fronty
// //         er.report( LOG_INFO, _("%s: Exceeded %d requests to redraw scene - drawing immediately.\n"), AT, c );
// //     }
}

void CRender::redrawScene()
{
    //Uint8 AniState = 0;
    //t_field fi;
    char buffer[50];

    /* zemetreseni
    static int a = 0, b = 0;
    a = rand() % 5;
    b = rand() % 5;

    Render::drawObject ( b_field[ga.getFieldNum()], a - 3, b - 3, false );
    */

//    CRender::drawObject ( b_field[ga.getFieldNum()], 0, 0, false );
    /* dummy score board */
    sprintf( buffer, "P0 %d:%d B%d F%d S%d K%d",
             players[0].getArrayX( players[0].getArrayMiddleX() ),
             players[0].getArrayY( players[0].getArrayMiddleY() ),
             players[0].getPowerup( PBOMB ),
             players[0].getPowerup( PFLAME ),
             players[0].getPowerup( PSKATE ),
             players[0].getPowerup( PKICKER )
           );
    drawString(  10,  7, buffer, cwhite, false );
    sprintf( buffer, "P1 %d:%d B%d F%d S%d K%d",
             players[1].getArrayX( players[1].getArrayMiddleX() ),
             players[1].getArrayY( players[1].getArrayMiddleY() ),
             players[1].getPowerup( PBOMB ),
             players[1].getPowerup( PFLAME ),
             players[1].getPowerup( PSKATE ),
             players[1].getPowerup( PKICKER )
           );
    drawString(  10, 27, buffer, cblack, false );

//     drawString( 110,  7, "S:0 K:0", cred,   false );
//     drawString( 110, 27, "S:0 K:0", cblue,  false );

//     drawString( 210,  7, "S:0 K:0", cgreen, false );
//     drawString( 210, 27, "S:0 K:0", cyellow, false );

//     drawString( 180,  7, "()[]{}:^#&@+-.!?|;%", ccyan, false );
//     drawString( 180, 27, "0123456789*$=\"<>~", cmagenta, false );

    sprintf( buffer, "%02d,%02d %02d,%02d %02d,%02d",
             players[0].getArrayMiddleX() % BOX_W,
             players[0].getArrayMiddleY() % BOX_H,
             abs( (players[0].getArrayMiddleX() % BOX_W) - BOX_W2 ),
             abs( (players[0].getArrayMiddleY() % BOX_H) - BOX_H2 ),
             abs( (players[0].getArrayMiddleX() % BOX_W) - BOX_W2 ) / players[0].getSpeedX(),
             abs( (players[0].getArrayMiddleY() % BOX_H) - BOX_H2 ) / players[0].getSpeedY()
           );
    drawString( 360, 7, buffer, cpurple, false );

    sprintf( buffer, "%2d,%2d",
             players[0].getSpeedX(),
             players[0].getSpeedY()
           );
    drawString( 360, 27, buffer, cpurple, false );

//     drawString( 360,  7, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", corange, false );
//     drawString( 360, 27, "abcdefghijklmnopqrstuvwxyz", cpurple, false );
/*
    for ( Uint8 y = 0; y < GA_H; ++y )
    {
        for ( Uint8 x = 0; x < GA_W; ++x )
        {
            fi = ga.getBoardField(x, y);

            if ( ga.getFieldNum() == 4 && fi.warp != NULL )
            {
                drawObject (
                    warps.getFrameSurface(cgreen),
                    warps.getScreenX() + x * BOX_W,
                    warps.getScreenY() + y * BOX_H,
                    false
                );
                continue; // dira musi byt videt
            }

            if ( fi.occupancy != TBLANK )
            {
                drawObject (
                    tiles[ga.getFieldNum()][fi.occupancy].getFrameSurface(cgreen),
                    tiles[ga.getFieldNum()][fi.occupancy].getScreenX() + x * BOX_W,
                    tiles[ga.getFieldNum()][fi.occupancy].getScreenY() + y * BOX_H,
                    false
                );
            }
            else
            {
                if ( ( ga.getFieldNum() == 2 || ga.getFieldNum() == 3 ) && fi.arrow != NULL )
                { // v originalnim bombicovi jsou sipky divne umistene --> bud je chyba
                     v propoctu, nebo to taky nejak fixluji :) 
                    drawObject (
                        arrows[ga.getBoardField(x,y).arrow->direction].getFrameSurface(cgreen),
                        arrows[ga.getBoardField(x,y).arrow->direction].getScreenX() + x * BOX_W + BOX_H2/2,
                        arrows[ga.getBoardField(x,y).arrow->direction].getScreenY() + y * BOX_H + BOX_H2,
                        false
                    );
                }

                if ( ga.getFieldNum() == 10 && fi.conveyor != NULL )
                {
                    drawObject (
                        conveyors[ga.getBoardField(x,y).conveyor->direction].getFrameSurface(cgreen),
                        conveyors[ga.getBoardField(x,y).conveyor->direction].getScreenX() + x * BOX_W,
                        conveyors[ga.getBoardField(x,y).conveyor->direction].getScreenY() + y * BOX_H,
                        false
                    );
                }

                if ( ga.getFieldNum() == 9 && fi.trampoline != NULL )
                {
                    drawObject (
                        trampolines.getFrameSurface(cgreen),
                        trampolines.getScreenX() + x * BOX_W,
                        trampolines.getScreenY() + y * BOX_H + BOX_H2,
                        false
                    );
                }

                if ( fi.powerup != -1 )
                {
                    drawObject(
                        powerups[fi.powerup].getFrameSurface(cgreen),
                        powerups[fi.powerup].getScreenX() + x * BOX_W,
                        powerups[fi.powerup].getScreenY() + y * BOX_H,
                        false
                    );
                }
            }
        }
    }
    
    for ( Uint8 i = 0; i < CBOMBS; i++ )
    {
        if ( bombs[i].getActive() )
        {
            CRender::drawObject(
                bombs[i].getFrameSurface( bombs[i].getColor() ),
                bombs[i].getSurfaceX() + bombs[i].getScreenX(),
                bombs[i].getSurfaceY() + bombs[i].getScreenY(),
                false
            );
        }
    }

    /// mame 10 barev 
    for ( Uint8 i = 0; i < CCOLORS; i++ )
    {
        if( players[i].getActive() )
        {
            AniState = players[i].getAniState();

            if ( AniState != SDIE && AniState != SSPIN )
            {
                CRender::drawObject(
                    players[i].getShadow(),
                    players[i].getSurShadowX() + players[i].getScreenX(),
                    players[i].getSurShadowY() + players[i].getScreenY(),
                    false
                );
            }

            CRender::drawObject(
                players[i].getFrameSurface( players[i].getColor() ),
                players[i].getSurfaceX() + players[i].getScreenX(),
                players[i].getSurfaceY() + players[i].getScreenY(),
                false
            );
        }
    }
   */

    SDL_Flip ( screen );
}

void CRender::setBG( SDL_Surface * background )
{
    this->background = background;
}

void CRender::clearScreen()
{
    assert ( screen != NULL );
    SDL_FillRect ( screen, NULL, SDL_MapRGB ( screen->format, 0, 127, 127 ) );
    SDL_Flip ( screen );
}

void CRender::clearRect( SDL_Rect* rect )
{
    assert ( screen != NULL );
    SDL_FillRect ( screen, rect, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void CRender::drawString(Sint16 x, Sint16 y, string str, bool flip)
{
    drawString( x, y, str, cwhite, flip );
}

void CRender::drawString(Sint16 x, Sint16 y, const char * str, bool flip)
{
    drawString( x, y, str, cwhite, flip );
}

void CRender::drawString(Sint16 x, Sint16 y, string str, imagecolors color, bool flip)
{
    Uint16 offset = 0;
    Sint16 spacing = 0;
    SDL_Surface * symbol = NULL;
    Uint8 s = 0;
    Sint16 size = 0;

    spacing = font.getSpacing() - 2; // 2 pix se umele pridavaji
//     spacing = -1;
    size = str.size();

    for ( Sint16 i = 0; i < size; ++i )
    {
        s = str[i];
        symbol = font.getSymbol(s, color);
        if ( symbol != NULL )
        {
            drawObject( symbol, x + offset, y, false );
            offset += font.getSymbolWidth(s);
            offset += spacing;
        }
    }

    if (flip)
    {
        SDL_Flip ( screen );
    }
}

void CRender::drawString(Sint16 x, Sint16 y, const char* str, imagecolors color, bool flip)
{
    Uint16 offset = 0;
    Sint16 spacing = 0;
    SDL_Surface * symbol = NULL;
    Uint8 s = 0;
    Sint16 size = 0;

    spacing = font.getSpacing() - 2; // 2 pix se umele pridavaji
//     spacing = font.getSpacing();
//     spacing = -1;
    size = strlen(str);

    for ( Sint16 i = 0; i < size; ++i )
    {
        s = str[i];
        symbol = font.getSymbol(s, color);
        if ( symbol != NULL )
        {
            drawObject( symbol, x + offset, y, false );
            offset += font.getSymbolWidth(s);
            offset += spacing;
        }
    }

    if (flip)
    {
        SDL_Flip ( screen );
    }
}


void CRender::drawLoadString(string str)
{
    drawLoadString(str.c_str());
    /*
    SDL_Rect r = {10, (Sint16)(getDimensionsOfScreen().h - 80), 620, 20 };
    clearRect( &r );

    drawString( 10, getDimensionsOfScreen().h - 80, str, true );
    */
}

void CRender::drawLoadString(const char* str)
{
    SDL_Rect r = {10, (Sint16)(getDimensionsOfScreen().h - 80), 620, 20};
    clearRect( &r );

    drawString( 10, getDimensionsOfScreen().h - 80, str, true );
}

void CRender::drawLoadString(string str, Uint16 status, Uint16 max)
{
    drawLoadString(str.c_str(), status, max);
    /*
    drawProgress ( 10, getDimensionsOfScreen().h - 50, 620, 40, ( 100.0 / max ) * ( status ) );
    drawLoadString( str );
     */
}

void CRender::drawLoadString(const char* str, Uint16 status, Uint16 max)
{
    drawProgress ( 10, getDimensionsOfScreen().h - 50, 620, 40, ( 100.0 / max ) * ( status ) );
    drawLoadString( str );
}


void CRender::drawObject ( SDL_Surface* img, Sint16 x, Sint16 y, bool flip )
{
    assert ( img != NULL );
    assert ( screen != NULL );
    
//     static Uint16 counter = 0;

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = img->w;
    rect.h = img->h;

    SDL_BlitSurface ( img, NULL, screen, &rect );
//     SDL_UpdateRect ( screen, 0, 0, screen->w, screen->h );
//     SDL_UpdateRect ( screen, rect.x, rect.y, rect.w, rect.h );

    if ( useOpenGl )
    { /* FIXIT! */
        SDL_GL_SwapBuffers();
    }
    else
    {
//         SDL_Flip ( screen );
    }
    
    if ( flip )
        SDL_Flip ( screen );
    
    
//     cout << "drawObject(" << ++counter << ")" << endl << flush;
}


void CRender::drawProgress ( Sint16 x, Sint16 y, Uint16 w, Uint16 h, float state )
{
    assert ( screen != NULL );
    SDL_Surface * bar = NULL;
    SDL_PixelFormat * pxlformat = NULL;
    SDL_Rect rect;

    rect.x = x + 1;
    rect.y = y + 1;
    rect.w = ( (Uint16)(( w / 100.0 ) * state) );
    rect.h = h - 2;

    pxlformat = SDL_GetVideoSurface()->format;

    bar = SDL_CreateRGBSurface ( SDL_HWSURFACE,
                                 rect.w, h,
                                 pxlformat->BitsPerPixel,
                                 pxlformat->Rmask,
                                 pxlformat->Gmask,
                                 pxlformat->Bmask,
                                 pxlformat->Amask );
    if ( bar == NULL )
    {
        er.report( LOG_ERROR, _("%s: CreateRGBSurface(): %s.\n"), AT, SDL_GetError() );
        return;
    }

//     printf( "%d, %x %x %x %x\n", pxlformat->BitsPerPixel, pxlformat->Rmask, pxlformat->Gmask, pxlformat->Bmask, pxlformat->Amask );

    if ( SDL_FillRect ( bar, NULL, SDL_MapRGB ( screen->format, 192, ((Uint8)state)*2, 0 ) ) )
    {
//         SDL_BlitSurface ( bar, NULL, screen, &rect );
        SDL_UpdateRect ( screen, rect.x, rect.y, rect.w, rect.h );
        return;
    }

//     if ( SDL_FillRect ( bar, &rect, SDL_MapRGB ( screen->format, 255, 255, 255 ) ) )
//     {
//         return;
//     }

    drawObject ( bar, x, y, true );
    SDL_FreeSurface ( bar );
}


SDL_Rect CRender::getDimensionsOfScreen()
{
    assert ( screen != NULL );
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = screen->w;
    r.h = screen->h;
    return r;
}


/*
SDL_PixelFormat* CRender::getPixelFormat()
{
    return screen->format;
}
*/

void CRender::changeAnimStateRandomPowerups(bool anim)
{
    if ( anim )
    {
        powerups[PRANDOM].startAnim();
    }
    else
    {
        powerups[PRANDOM].stopAnim();
    }
}


CRender::CRender()
{
    er.setLevel( LOG_INFO );
    
    win_flags = 0;
    screen = NULL;   // Okno
    useOpenGl = false;
    background = NULL;
//     objSize = objects.size();

//     mutRedraw = SDL_CreateMutex();
//     if ( mutRedraw == NULL )
//     {
//         cerr << SDL_GetError() << endl << flush;
//         exit(1);
//     }

    pthread_mutexattr_settype(&mutRedrawAttr, PTHREAD_MUTEX_RECURSIVE);
    
    if ( pthread_mutex_init ( &mutRedraw, &mutRedrawAttr ) != 0 )
//     if ( MUTEX_INIT(&mutRedraw) != 0 )
    {
        er.report( LOG_ERROR, _("%s: Couldn't create mutex."), AT );
        exit(1);
    }

//     pthread_mutex_init ( &mutRedraw, NULL );
    timerOperationRedrawCounter = 0;

    atexit ( SDL_Quit );
}

CRender::~CRender()
{
    int i = 0;

    /* TODO Destroy mutex */
//     SDL_DestroyMutex( mutRedraw );
//     pthread_mutex_destroy ( &mutRedraw );
    
    SDL_FreeSurface ( b_drawgame );
    SDL_FreeSurface ( b_bonusgame );
    SDL_FreeSurface ( b_results );
    SDL_FreeSurface ( b_mainmenu );
    SDL_FreeSurface ( b_team[0] );
    SDL_FreeSurface ( b_team[1] );
    for ( i = 0; i < CCOLORS; ++i )
    {
        SDL_FreeSurface ( b_victory[i] );
    }
    for ( i = 0; i < CBACKGROUNDS; ++i )
    {
        SDL_FreeSurface ( b_glue[i] );
    }
    for ( i = 0; i < CFIELDS; ++i )
    {
//        SDL_FreeSurface ( b_field[i] );
    }
}
