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

#include <iostream>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <string>
#include <pthread.h>

#include "core/animation.h"
#include "readers/anireader.h"
#include "readers/alireader.h"
#include "core/seqparser.h"
#include "readers/pcxreader.h"
#include "readers/resreader.h"
#include "render/render.h"
#include "core/go_bomb.h"
#include "core/go_player.h"
#include "global.h"
#include "readers/cfgreader.h"
#include "core/gamingarea.h"
#include "readers/fonreader.h"

#include "core/scheduler.h"

#if defined(CONF_PF_LINUX)
#include <X11/Xlib.h>
#endif

using namespace std;

/** GLOBAL {{{ */



/* Animations.*/


CRender render;

CGO_Player players[CCOLORS];
CGO_Bomb   bombs[CBOMBS];



bool gameRun = false;
// bool loadingFiles = false;
float loadingProgress = 0.0;
// AniReader *loadingAnim;

/** }}} */

#define ERROR_LOAD "Behem nacitani doslo k chybe. Nelze pokracovat."

int MUTEX_INIT(MUTEX *mutex)
{
#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
	return pthread_mutex_init (mutex, NULL);
#elif defined(CONF_PF_WINDOWS)
	*mutex = CreateMutex(0, FALSE, 0);
	return (*mutex==0);
#endif
	return -1;
}

int MUTEX_LOCK(MUTEX *mutex)
{
#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
	return pthread_mutex_lock( mutex );
#elif defined(CONF_PF_WINDOWS)
	return (WaitForSingleObject(*mutex, INFINITE)==WAIT_FAILED?1:0);
#endif
	return -1;
}

int MUTEX_UNLOCK(MUTEX *mutex)
{
#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
	return pthread_mutex_unlock( mutex );
#elif defined(CONF_PF_WINDOWS)
	return (ReleaseMutex(*mutex)==0);
#endif
	return -1;
}


bool ProcessEvent()
{
	static int events = 0;

	SDL_Event event;

	while (SDL_WaitEvent(&event))
	{
		events++;
		switch (event.type)
		{


			// Stisk klavesy
//		 case SDL_KEYDOWN:
//			 switch (event.key.keysym.sym)
//			 {
//				 // Konec programu
// //			 case SDLK_ESCAPE:
//			 case SDLK_q:
//				 render.PushQuitEvent();
//				 break;
//
//				 // Zapne/vypne ignorovani mysi
// //			 case SDLK_SPACE:
// // //				 render.ChangeEventState(SDL_KEYUP);
// // //				 render.ChangeEventState(SDL_MOUSEBUTTONDOWN);
// // //				 render.ChangeEventState(SDL_MOUSEBUTTONUP);
// //				 break;
//
//
// //			 case SDLK_COMMA:
// //				 if( --im.c < 0 )
// //					 im.c = 9;
// //				 LoadImages( im.c );
// //				 PushRedrawEvent();
// //				 break;
//
// //			 case SDLK_PERIOD:
// //				 if( ++im.c > 9 )
// //					 im.c = 0;
// //				 LoadImages( im.c );
// //				 PushRedrawEvent();
// //				 break;
//
// //			 case SDLK_PAGEUP:
// //				 printf("pageup\n");
// //				 break;
// //
// //			 case SDLK_PAGEDOWN:
// //				 printf("pagedown\n");
// //				 break;
//
//				 // Minimalizace okna
// //		  case SDLK_m:
// //			  SDL_WM_IconifyWindow();
// //			  break;
//
//			 default:
// // //			  printf("(%c) ", event.key.keysym.sym); fflush(stdout);
//				 break;
//			 }
//			 break;

			// Uvolneni klavesy, pouze vypis do konzole, ze nastala
//		 case SDL_KEYUP:
//			 if ( players[0].getRunState() != XSTAND )
//				 players[0].setRunStateSafe( XSTAND );
//
//			 if ( players[1].getRunState() != XSTAND )
//				 players[1].setRunStateSafe( XSTAND );
//
//			 render.PushRedrawEvent();
//			 break;

			// Udalosti mysi, nemely by nikdy nastat (viz EventFilter())
//		 case SDL_MOUSEMOTION:
//			 printf("SDL_MOUSEMOTION ");
//			 break;
//		 case SDL_MOUSEBUTTONDOWN:
//			 printf("SDL_MOUSEBUTTONDOWN ");
//			 break;
//		 case SDL_MOUSEBUTTONUP:
//			 printf("SDL_MOUSEBUTTONUP ");
//			 break;

			// Zmena velikosti okna
//		 case SDL_VIDEORESIZE:
//			 g_screen = SDL_SetVideoMode(event.resize.w, event.resize.h, WIN_BPP, g_win_flags);
//
//			 if(g_screen == NULL)
//			 {
//				 fprintf(stderr, "Unable to resize window: %s\n", SDL_GetError());
//				 return false;
//			 }
//			 break;

		case REDRAW:
//			 cout << "REDRAW(" << events << ")" << endl << flush;
			render.redrawScene();
			break;

		case SDL_VIDEOEXPOSE:
			printf("SDL_VIDEOEXPOSE ");
			break;

			// Pozadavek na ukonceni
		case SDL_QUIT:
//			 cout << "SDL_QUIT(" << events << ")" << endl << flush;
			return false;
			break;

		default:
//		  printf("[%d] ", event.type);
			break;
		}

		cout << flush;
	}

	return true;
}

// void *loadingAnim_thread(void *vargp)
// {
//	 int *ptr = (int*)vargp;
//	 Uint8 frame = 0;
//	 sequence anim = loadingAnim->getSeq(1);
//	 SDL_Rect rect;
//	 rect.x = 280;
//	 rect.y = 170;
//	 rect.h = 150;
//	 rect.w = 80;
//
//	 while ( loadingFiles )
//	 {
//		 render.clearRect( &rect );
//		 render.drawObject(
//			 (loadingAnim->getFrm()[0])[anim.stats[frame++].frams[0].frames[0].imgno].img.im[0],
//			 280,
//			 170,
//			 true
//		 );
//
//		 render.drawProgress ( 10, render.getDimensionsOfScreen().h - 50, 620, 40, loadingProgress );
//
//		 if ( frame >= anim.head.states )
//			 frame = 0;
//
//		 SDL_Delay(200);
//	 }
//
//	 pthread_exit((void*)*ptr);
// }


string getText()
{
	SDL_Event keyevent;	//The SDL event that we will poll to get events.
	string strInputBuffer = "";	// This is where you will store typed keys

	while (SDL_WaitEvent(&keyevent))   //Poll our SDL key event for any keystrokes.
	{
		//if( keyevent.type == SDL_KEYDOWN ) // Check if this key is being released
			// Check if the key is in the upper or lowwer case ASCII ranges
			//if( (keyevent.key.keysym.sym >= 65 && keyevent.key.keysym.sym <= 90) ||
			//		(keyevent.key.keysym.sym >= 97 && keyevent.key.keysym.sym <= 122) )
				/** This is either an upper or lowwer case letter,
				*  add it to the buffer, This check can obviously
				*  be easily modified to allow for numbers and more.
				*  But it is important to prevent the ASCII values
				*  of control keys or others from being nonsensically
				*  added to your buffer.
				*/
				//strInputBuffer += reinterpret_cast<char>( (char)keyevent.key.keysym.sym );
	}
	return strInputBuffer;
}


void *key_thread(void *vargp)
{
	int *ptr = (int*)vargp;
	Uint8* keystates = SDL_GetKeyState(NULL);

	bool done = false;
	bool rd = false;

	bool buttonActionDown[2] = {false, false};

	bool consoleIsActive = false;

	while ( !done )
	{
		SDL_PumpEvents();

		rd = false;

		if ( consoleIsActive )
		{

		}
		else
		{


			if (keystates[SDLK_s])
			{
				players[1].movePlayerRel( AWEST, ANONE );
				rd = true;
			}
			else
			{
				if (keystates[SDLK_d])
				{
					players[1].movePlayerRel( ASOUTH, ANONE );
					rd = true;
				}
				else
				{
					if (keystates[SDLK_f])
					{
						players[1].movePlayerRel( AEAST, ANONE );
						rd = true;
					}
					else
					{
						if (keystates[SDLK_e])
						{
							players[1].movePlayerRel( ANORTH, ANONE );
							rd = true;
						}
						else
						{
							if ( players[1].getRunState() == XWALK )
							{
								players[1].setRunStateSafe( XSTAND );
								rd = true;
							}
						}
					}
				}
			}


//		 if (keystates[SDLK_LEFT])
//		 {
//			 if ( players[0].movePlayerRel( AWEST ) == -1 )
//			 {
//				 if (keystates[SDLK_DOWN] || keystates[SDLK_UP])
//				 {
//					 if ( keystates[SDLK_DOWN] )
//					 {
//						 players[0].movePlayerRel( ASOUTH );
//					 }
//					 else
//					 {
//						 players[0].movePlayerRel( ANORTH );
//					 }
//				 }
//			 }
//			 rd = true;
//		 }
			if (keystates[SDLK_LEFT])
			{
				if (keystates[SDLK_DOWN] || keystates[SDLK_UP])
				{
					if ( keystates[SDLK_DOWN] )
					{
						players[0].movePlayerRel( AWEST, ASOUTH );
					}
					else
					{
						players[0].movePlayerRel( AWEST, ANORTH );
					}
				}
				else
				{
					players[0].movePlayerRel( AWEST, ANONE );
				}
			}
			else
			{
				if (keystates[SDLK_DOWN])
				{
					if (keystates[SDLK_LEFT] || keystates[SDLK_RIGHT])
					{
						if ( keystates[SDLK_LEFT] )
						{
							players[0].movePlayerRel( ASOUTH, AWEST );
						}
						else
						{
							players[0].movePlayerRel( ASOUTH, AEAST );
						}
					}
					else
					{
						players[0].movePlayerRel( ASOUTH, ANONE );
					}
				}
				else
				{
					if (keystates[SDLK_RIGHT])
					{
						if (keystates[SDLK_DOWN] || keystates[SDLK_UP])
						{
							if ( keystates[SDLK_DOWN] )
							{
								players[0].movePlayerRel( AEAST, ASOUTH );
							}
							else
							{
								players[0].movePlayerRel( AEAST, ANORTH );
							}
						}
						else
						{
							players[0].movePlayerRel( AEAST, ANONE );
						}
					}
					else
					{
						if (keystates[SDLK_UP])
						{
							if (keystates[SDLK_LEFT] || keystates[SDLK_RIGHT])
							{
								if ( keystates[SDLK_LEFT] )
								{
									players[0].movePlayerRel( ANORTH, AWEST );
								}
								else
								{
									players[0].movePlayerRel( ANORTH, AEAST );
								}
							}
							else
							{
								players[0].movePlayerRel( ANORTH, ANONE );
							}
						}
						else
						{
							if ( players[0].getRunState() == XWALK )
							{
								players[0].setRunStateSafe( XSTAND );
								rd = true;
							}
						}
					}
				}
			}

			if ( keystates[SDLK_a] )
			{
				if (buttonActionDown[1] == false)
				{
					buttonActionDown[1] = true;


					if ( render.ga.getBoardField( players[1].getArrayX( players[1].getArrayMiddleX() ),
												  players[1].getArrayY( players[1].getArrayMiddleY() )).bomb == false
							&& render.ga.getBoardField( players[1].getArrayX( players[1].getArrayMiddleX() ),
														players[1].getArrayY( players[1].getArrayMiddleY() )).occupancy == TBLANK )
					{
						if ( players[1].dropBomb() )
						{
							for ( Uint16 i = 0; i < CBOMBS; i++ )
							{
								if ( !bombs[i].getActive() )
								{
									bombs[i].setActive(true);
									bombs[i].setColor( players[1].getColor() );
									bombs[i].setRelX( players[1].getArrayX( players[1].getArrayMiddleX() ) * BOX_W );
									bombs[i].setRelY( players[1].getArrayY( players[1].getArrayMiddleY() ) * BOX_H );
									bombs[i].startAnim();
									render.ga.setFieldValBomb( players[1].getArrayX( players[1].getArrayMiddleX() ),
															   players[1].getArrayY( players[1].getArrayMiddleY() ), true );
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				 buttonActionDown[1] = false;
			}

//		 if ( keystates[SDLK_n] )
//		 {
//			 for ( Uint16 i = 0; i < CBOMBS; i++ )
//			 {
//				 if ( !bombs[i].getActive() )
//				 {
// //					 cout << (int)i << " je aktivovana" << endl;
//					 bombs[i].setActive(true);
//					 bombs[i].setColor( players[0].getColor() );
//					 bombs[i].setRelX( players[0].getArrayX() * BOX_W );
//					 bombs[i].setRelY( players[0].getArrayY() * BOX_H );
//					 bombs[i].startAnim();
//					 break;
//				 }
//			 }
//		 }


			if (keystates[SDLK_y])
			{
				players[0].setAniDirState ( SSTAND, ANONE );
				rd = true;
			}

			if (keystates[SDLK_x])
			{
				players[0].setAniDirState ( SCORNERHEAD, ANONE );
				rd = true;
			}

			if (keystates[SDLK_c])
			{
				players[0].setAniDirState ( SDIE, ANONE );
				rd = true;
			}

			if (keystates[SDLK_v])
			{
				players[0].setAniDirState ( SPUNCH, ANONE );
				rd = true;
			}

			if (keystates[SDLK_b])
			{
				players[0].setAniDirState ( SKICK, ANONE );
				rd = true;
			}
			if (keystates[SDLK_n])
			{
				players[0].setAniDirState ( SPICKUP, ANONE );
				rd = true;
			}
			if (keystates[SDLK_m])
			{
				players[0].setAniDirState ( SSPIN, ANONE );
				rd = true;
			}

			if ( rd )
			{
//			 render.PushRedrawEvent();
			}

			if (keystates[SDLK_ESCAPE])
			{
				SDL_EventState(REDRAW, SDL_IGNORE); // ignorovat pozadavky na prekresleni

				for ( Uint8 i = 0; i < CCOLORS; i++ )
				{
					players[i].stopAnim();
					players[i].setActive(false);
				}
				for ( Uint8 i = 0; i < CBOMBS; i++ )
				{
					bombs[i].stopAnim();
					bombs[i].setActive(false);
				}
				done = true;
				render.PushQuitEvent();

//			 SDL_EventState(REDRAW, SDL_ENABLE);
			}

		}

		if( keystates[SDLK_i] )
		{
			cout << "getText: " << getText() << endl;

//			 consoleIsActive = !consoleIsActive;
		}



		SDL_Delay(KEYREPEAT);

	}

	gameRun = false;
	pthread_exit((void*)*ptr);
}


int main2 ( int argc, char **argv )
{
//	 debugLevel = LOG_INFO;

	CAliReader ali;
	CAniReader *anis;
	CSeqParser seq;
	CResReader options;
	CCfgReader cfg;

	t_player player;

//	 Scheduler sch;
//	 schRecord item;
//	 SchedulerItem schI;


//	 SDL_Event event;
//	 Uint32 keypress = 0;

	Uint16 i;
//	 SDL_Surface *img;

	int c;
	bool useOpenGL = false;
//	 bool fullscreen = true;
//	 extern char *optarg;
//	 extern int optind, optopt, opterr;
	while ( ( c = getopt ( argc, argv, ":dgw" ) ) != -1 )
	{
		switch ( c )
		{
		case 'd':
			//debug = true;
			break;
		case 'g':
			useOpenGL = true;
			break;
		case 'w':
//			 fullscreen = false;
			break;
		}
	}

	setlocale(LC_ALL, "");
	bindtextdomain("codenamebomb", "./locale");
	textdomain("codenamebomb");

	/* read config file; load default value, if not exists */
	cfg.readConfigFile();

#if defined(CONF_PF_LINUX)
	XInitThreads();
#endif

	render.init ( useOpenGL, &cfg );

	SDL_ShowCursor(SDL_DISABLE);


//	 loadingFiles = true;
//	 loadingAnim = new AniReader;

//	 render.drawLoadString( "DATA/ANI/BOMBWALK.ANI" );
//	 loadingAnim->read( "DATA/ANI/BOMBWALK.ANI" );


//	 pthread_t tidLoadingAnim;
//	 if ( pthread_create(&tidLoadingAnim, NULL, loadingAnim_thread, (void*)&i) != 0 )
//	 {
//		 cerr << "Nelze vytvorit vlakno (animace nacitani)!" << endl;
//		 return 1;
//	 }


//	 anis = new AniReader[1];
//	 anis[0].read ( "DATA/ANI/CLASSICS.ANI" );


//	 options.read( "DATA/RES/VALUELST.RES" );
//
//	 cout << "3: " << options.getItem( 3 ) << endl;

	render.drawLoadString( "DATA/ANI/MASTER.ALI" );
	if ( ali.read ( "DATA/ANI/MASTER.ALI" ) )
	{
		cerr << endl << ERROR_LOAD << " -- !" << endl;
		return 1;
	}

	Uint16 anifiles = ali.getCount();

	anis = new CAniReader[anifiles];

	for ( i = 0; i < anifiles; ++i )
	{
//		 render.drawObject ( b_mainmenu, 0, 0 ); // only for debug!

#if defined(CONF_PF_LINUX) || defined(CONF_PT_MACOSX)
		if ( pthread_yield() < 0 )
		{
			fprintf(stderr, "pthread_yield() error!\n");
		}
#elif defined(CONF_PF_WINDOWS)
		SwitchToThread();
#endif


		render.drawLoadString( "DATA/ANI/" + ali.getItem ( i ), i, anifiles + 1 );

		if ( anis[i].read ( "DATA/ANI/" + ali.getItem ( i ) ) )
		{
			cerr << endl << ERROR_LOAD << endl;
			delete[] anis;
			return 1;
		}

		Uint16 seqs = anis[i].getSeqCount();
		for ( Uint16 a = 0; a < seqs; ++a )
		{
//			 cout << "(*(anis[i].getFrm())).[]: " << (*(anis[i].getFrm()))[0].file.name << endl;

			switch ( seq.assign ( anis[i].getSeq ( a ), &(*(anis[i].getFrm())) ) )
			{
			case PREPARE:
				anis[i].prepareColor( cyellow );
				anis[i].prepareColor( ccyan );
				anis[i].prepareColor( cred );
				anis[i].prepareColor( cblue );
				anis[i].prepareColor( corange );
				anis[i].prepareColor( cmagenta );
				anis[i].prepareColor( cpurple );
				anis[i].prepareColor( cwhite );
				anis[i].prepareColor( cblack );
				break;
			case NOTPREPARE:
				break;
			};
		}
	}


//	 loadingFiles = false;
//	 pthread_join(tidLoadingAnim, NULL);
//	 delete loadingAnim;

	//render.drawObject ( b_mainmenu, 0, 0 );

//	 cout << endl << "Nacitani probehlo v poradku. Cekam na libovolnou klavesu a pak koncim." << endl;
//
//	 while ( !keypress )
//	 {
//		 while ( SDL_PollEvent ( &event ) )
//		 {
//			 switch ( event.type )
//			 {
//			 case SDL_QUIT:
//				 keypress = 1;
//				 break;
//			 case SDL_KEYDOWN:
//				 keypress = 1;
//				 break;
//			 }
//		 }
//	 }


//	 item.schItem = &schI;
//	 item.time = 1000 + SDL_GetTicks();
//	 cout << sch.addItem( &item ) << endl;

//	 render.setBG( b_field[0] );


	player = render.ga.getPlayerInfo( 0 );

	players[0].setType(GOT_PLAYER);
	players[0].setName("KEY 0");
	players[0].setActive(true);
	players[0].setColor( cmagenta );
	players[0].setArrayX( player.x );
	players[0].setArrayY( player.y );


	player = render.ga.getPlayerInfo( 1 );

	players[1].setType(GOT_PLAYER);
	players[1].setName("KEY 1");
	players[1].setActive(true);
	players[1].setColor( cblue );
	players[1].setArrayX( player.x );
	players[1].setArrayY( player.y );

//.... fiels for loops


	render.ga.setFieldValPowerup( 0, 0, PBOMB );

	render.ga.setFieldValPowerup( 5, 4, PFLAME );
	render.ga.setFieldValPowerup( 6, 4, PSKATE );
	render.ga.setFieldValPowerup( 7, 4, PKICKER );
	render.ga.setFieldValPowerup( 8, 4, PJELLY );

	render.ga.setFieldValPowerup( 5, 5, PTRIGGER );
	render.ga.setFieldValPowerup( 6, 5, PPUNCH );
	render.ga.setFieldValPowerup( 7, 5, PGRAB );
	render.ga.setFieldValPowerup( 8, 5, PSPOOGE );

	render.ga.setFieldValPowerup( 5, 6, PSKATE );
	render.ga.setFieldValPowerup( 6, 6, PSKATE );
	render.ga.setFieldValPowerup( 7, 6, PSKATE );
	render.ga.setFieldValPowerup( 8, 6, PSKATE );

	render.ga.setFieldValPowerup( 5, 7, PSKATE );
	render.ga.setFieldValPowerup( 6, 7, PSKATE );
	render.ga.setFieldValPowerup( 7, 7, PRANDOM );
	render.changeAnimStateRandomPowerups (true);
	render.ga.setFieldValPowerup( 8, 7, PSKATE );

	render.ga.setFieldValPowerup( 5, 8, PSKATE );
	render.ga.setFieldValPowerup( 6, 8, PSKATE );
	render.ga.setFieldValPowerup( 7, 8, PSKATE );
	render.ga.setFieldValPowerup( 8, 8, PSKATE );


	//cout << "m: " << getText() << endl;

	gameRun = true;
	pthread_t tidKeys;
	pthread_create(&tidKeys, NULL, key_thread, (void*)&i);

	while (gameRun)
	{
		render.redrawScene();
		gameRun = ProcessEvent();
	}

	pthread_join(tidKeys, NULL);
	SDL_ShowCursor(SDL_ENABLE);


	// konec
	delete[] anis;


	return 0;
}












