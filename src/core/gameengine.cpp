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
#include <def/files.h>
#include <def/version.h>

#include <game/programstate.h>
#include <game/buttonlistener.h>

#include <core/gamestate.h>
#include <core/gameengine.h>

#include <cassert>

#include <SDL/SDL_mixer.h>

#if defined(CONF_PF_LINUX)
#include <X11/Xlib.h>
#endif

/**
 * DEBUG
 */

#include <network/localclient.h>
#include <network/localserver.h>
#include <network/networkclient.h>

/**
 * END DEBUG
 */


CGameEngine * CGameEngine::s_instance;

/* This is a way of telling whether or not to use hardware surfaces */
Uint32 CGameEngine::FastestFlags (Uint32 flags, unsigned width, unsigned height, unsigned bpp)
{
	const SDL_VideoInfo *info;

	/* Hardware acceleration is only used in fullscreen mode */
	flags |= SDL_FULLSCREEN;

	/* Check for various video capabilities */
	info = SDL_GetVideoInfo();
	if (info->blit_hw_CC && info->blit_fill)
	{
		/* We use accelerated colorkeying and color filling */
		flags |= SDL_HWSURFACE;
	}
	/* If we have enough video memory, and will use accelerated
	   blits directly to it, then use page flipping.
	 */
	if ((flags & SDL_HWSURFACE) == SDL_HWSURFACE)
	{
		/* Direct hardware blitting without double-buffering
		   causes really bad flickering.
		 */
		if (info->video_mem * 1024 > (height * width * bpp / 8))
		{
			flags |= SDL_DOUBLEBUF;
		}
		else
		{
			flags &= ~SDL_HWSURFACE;
		}
	}

	/* Return the flags */
	return (flags);
}

void CGameEngine::Init (const char* title, int width, int height, int bpp, Uint32 sdlFlags)
{
	er.report(LOG_INFO, _("%s: Starting %s engine...\n"), AT, title);

	er.report(LOG_INFO, _("%s: Data dir=%s\n"), AT, DATA_DIR);
	er.report(LOG_INFO, _("%s: User dir=%s\n"), AT, USER_DIR);

	// Multithreaded version of X librares
#if defined(CONF_PF_LINUX)
	XInitThreads();
#endif

	SDL_version sdlVersion;
	SDL_VERSION(&sdlVersion);

	er.report(LOG_INFO, _("%s: SDL version is %d.%d.%d\n"), AT, (Uint16) sdlVersion.major, (Uint16) sdlVersion.minor, (Uint16) sdlVersion.patch);


	// initialize SDL
	if (SDL_Init(SDL_SUBSYSTEMS) == -1)
	{
		er.report(LOG_ERROR, _("%s: Unable to initialize SDL: %s!\n"), AT, SDL_GetError());
		throw;
	}

	/*
		On win32 it can be useful to call SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)
		at the start of your program. This will push the resolution of SDL_Delay into the millisecond
		range. Normally, it has a resolution of about 10ms.
	*/

	// set the title bar text
	SDL_WM_SetCaption(title, title);
	//Center the game Window
	char env [] = "SDL_VIDEO_CENTERED=center";
	SDL_putenv(env);

	// create the screen surface
	mScreen = SDL_SetVideoMode(width, height, bpp, sdlFlags);
	if (mScreen == NULL)
	{
		er.report(LOG_ERROR, _("%s: Unable to set %dx%d video resolution: %s!\n"), AT, width, height, SDL_GetError());
		throw;
	}

	// Print out information about our surfaces
	er.report(LOG_INFO, _("%s: Screen is at %d bits per pixel\n"), AT, mScreen->format->BitsPerPixel);
	if ((mScreen->flags & SDL_HWSURFACE) == SDL_HWSURFACE)
	{
		er.report(LOG_INFO, _("%s: Screen is in video memory\n"), AT);
	}
	else
	{
		er.report(LOG_INFO, _("%s: Screen is in system memory\n"), AT);
	}
	if ((mScreen->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF)
	{
		er.report(LOG_INFO, _("%s: Screen has double-buffering enabled\n"), AT);
	}

	if ((mScreen->flags & SDL_FULLSCREEN) == SDL_FULLSCREEN)
	{
		mFullscreen = true;
	}
	else
	{
		mFullscreen = false;
	}

	mRunning = true;

	// We want unicode
	SDL_EnableUNICODE(1);
	// We want to enable key repeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// AUDIO
	if ( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		er.report(LOG_ERROR, _("%s: Error initializing SDL_mixer: %s\n"), AT, Mix_GetError());
	}

	// NETWORK
	StartLocalServer();
	client = NULL;

	/*
	 * Now it's time for Guichan SDL stuff
	 */
	mImageLoader = new gcn::SDLImageLoader();
	// The ImageLoader in use is static and must be set to be
	// able to load images
	gcn::Image::setImageLoader(mImageLoader);
	mGraphics = new gcn::SDLGraphics();
	// Set the target for the graphics object to be the screen.
	// In other words, we will draw to the screen.
	// Note, any surface will do, it doesn't have to be the screen.
	mGraphics->setTarget(mScreen);
	gcnInput = new gcn::SDLInput();

	/*
	 * Last but not least it's time to initialize and create the gui
	 * with Guichan stuff.
	 */
	/////gui = new gcn::Gui();
	// Set gui to use the SDLGraphics object.
	/////gui->setGraphics(graphics);
	// Set gui to use the SDLInput object
	/////gui->setInput(gcnInput);
	// Set the top container
	//gui->setTop(top);
	// Load the image font.
	// DATA_DIR "gui/font.png"
	mFont = new gcn::ImageFont(PATH_FILE_GUI_FONT, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
	// The global font is static and must be set.
	gcn::Widget::setGlobalFont(mFont);

	mXmlGui = new XmlGui();
	mXmlGui->parse(PATH_FILE_GUI_XML);

	for (Uint8 i = 0; i < CGUISTATES; i++)
	{
		guis[i] = new gcn::Gui;
		guis[i]->setGraphics(mGraphics);
		guis[i]->setInput(gcnInput);

		LoadTopGUI(i);
	}

	mButtonActionListener = new CButtonListener(this);
	InitButtons();

	ChangeTopGUI(guiScreen::mainMenu);

	er.report(LOG_INFO, _("%s: CGameEngine Init\n"), AT);
}

void CGameEngine::InitButtons ()
{
	InitButton("0-start-game");
	InitButton("0-start-network-game");
	InitButton("0-join-network-game");
	InitButton("0-settings");
	InitButton("0-bomberman");
	InitButton("0-manual");
	InitButton("0-exit-bomberman");

	InitButton("1-back");
	InitButton("1-next");
	InitButton("1-player1-dd");
	InitButton("1-player2-dd");
	InitButton("1-player3-dd");
	InitButton("1-player4-dd");
	InitButton("1-player5-dd");
	InitButton("1-player6-dd");
	InitButton("1-player7-dd");
	InitButton("1-player8-dd");
	InitButton("1-player9-dd");
	InitButton("1-player10-dd");

	InitButton("2-back");
	InitButton("2-next");

	InitButton("3-back");
	InitButton("3-scheme-dd");

	InitButton("4-back");

	InitButton("5-back");
	InitButton("5-game-server-b");

	InitButton("6-back");
}

void CGameEngine::InitButton (const char * id)
{
	gcn::Widget * w = NULL;

	if ((w = mXmlGui->getWidget(id)) != NULL)
	{
		w->setActionEventId(id);
		w->addActionListener(mButtonActionListener);
	}
}

void CGameEngine::Cleanup ()
{
	// cleanup the all states
	while (!mStates.empty())
	{
		mStates.back()->Cleanup();
		mStates.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
	if (mFullscreen)
	{
		mScreen = SDL_SetVideoMode(640, 480, 0, 0);
	}

	delete mXmlGui;
	delete mFont;
	//delete top; ???
	delete gui;

	/*
	 * Destroy Guichan SDL stuff
	 */
	delete gcnInput;
	delete mGraphics;
	delete mImageLoader;

	delete mButtonActionListener;

	er.report(LOG_INFO, _("%s: CGameEngine Cleanup\n"), AT);

	// AUDIO
	Mix_CloseAudio();

	// NETWORK
	SDLNet_Quit();

	// shutdown SDL
	SDL_Quit();
}

void CGameEngine::ChangeState (CProgramState* state)
{
	// cleanup the current state
	if (!mStates.empty())
	{
		mStates.back()->Cleanup();
		mStates.pop_back();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->Init();
}

void CGameEngine::PushState (CProgramState* state)
{
	// pause current state
	if (!mStates.empty())
	{
		mStates.back()->Pause();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->Init();
}

void CGameEngine::PopState ()
{
	// cleanup the current state
	if (!mStates.empty())
	{
		mStates.back()->Cleanup();
		mStates.pop_back();
	}

	// resume previous state
	if (!mStates.empty())
	{
		mStates.back()->Resume();
	}
}

void CGameEngine::HandleEvents ()
{
	// let the state handle events
	mStates.back()->HandleEvents();
}

void CGameEngine::HandleNetwork()
{
	/* client (local or network) thinking ...*/

	try
	{
		getClient()->think();
	}
	catch( int e )
	{
		cout << "client e: " << e << endl;
		if (e == 1)
		{ // network error
			er.report(LOG_WARNING, _("%s: Network failure. Go to main menu.\n"), AT);
			StartLocalClient();
			CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
		}
	}

	try
	{
		getServer()->think();
	}
	catch( int e )
	{
		cout << "server e: " << e << endl;
		if (e == 1)
		{ // network error
			er.report(LOG_WARNING, _("%s: Network failure. Go to main menu.\n"), AT);
			getServer()->stopNetwork();
			CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
		}
	}


	/* if server (network) exists, then thinking...
	if(getServer() != NULL)
	{
		try
		{
			getServer()->think();
		}
		catch( int e )
		{
			cout << "server e: " << e << endl;
		}
	}
	*/
}

void CGameEngine::Update ()
{
	// let the state update the game
	mStates.back()->Update();
}

void CGameEngine::Draw ()
{
	// let the state draw the screen
	mStates.back()->Draw();
}

void CGameEngine::Run()
{
	const Uint32 TICKS_PER_SECOND = 50;
	const Uint32 SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const Uint32 MAX_FRAMESKIP = 10;

	Uint32 next_game_tick = SDL_GetTicks();
	Uint32 loops = 0;


	/* DEBUG */
/*
	CLocalClient * lc = new CLocalClient();
	lc->test_sendText("XXX");

	lc->movePlayer(cred, 10, 10, 20, 10);
*/
	/* END DEBUG */

	while (Running())
	{
		loops = 0;
		while( SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP )
		{
			HandleEvents();
			HandleNetwork();
			Update();

			next_game_tick += SKIP_TICKS;
			loops++;
		}
		Draw();
	}
}

void CGameEngine::StopClient()
{
	if (client != NULL)
	{
		delete client;
		client = NULL;
		er.report(LOG_INFO, _("%s: Client stopped.\n"), AT);
	}
	else
	{
		er.report(LOG_WARNING, _("%s: Client not running!\n"), AT);
	}
}

void CGameEngine::StartLocalClient()
{
	/* any client running? */
	if (client != NULL)
	{
		/* stop it */
		StopClient();
	}

	er.report(LOG_INFO, _("%s: Local client starting.\n"), AT);

	// local game -- set the networkgame state to false
	CGameState::instance()->setNetworkGame(false);

	/* start new local client */
	client = new CLocalClient();
}

/* test if server running! */
void CGameEngine::StartNetworkClient(const char * hostname)
{
	/* any client running? */
	if (client != NULL)
	{
		/* stop it */
		StopClient();
	}

	er.report(LOG_INFO, _("%s: Network client starting.\n"), AT);

	// network game -- set the networkgame state to true
	CGameState::instance()->setNetworkGame(true);

	/* start new local client */
	client = new CNetworkClient(hostname);
}

void CGameEngine::StopServer()
{
	if (server != NULL)
	{
		delete server;
		server = NULL;
		er.report(LOG_INFO, _("%s: Server stopped.\n"), AT);
	}
	else
	{
		er.report(LOG_WARNING, _("%s: Server not running!\n"), AT);
	}
}

void CGameEngine::StartNetworkServer()
{
	if (server == NULL)
	{
		StartLocalServer();
	}

	server->startNetwork();
}

void CGameEngine::StopNetworkServer()
{
	if(server != NULL)
	{
		server->stopNetwork();
	}
}

void CGameEngine::StartLocalServer()
{
	/* any server running? */
	if (server != NULL)
	{
		/* stop it */
		StopServer();
	}

	er.report(LOG_INFO, _("%s: Local server starting.\n"), AT);

	/* start new local server */
	server = new CLocalServer();
}

IClient * CGameEngine::getClient()
{
	//assert(netClient != NULL);
	if (client == NULL)
	{
		er.report(LOG_WARNING, _("%s: No client defined, creating local.\n"), AT);
		StartLocalClient();
	}

	return client;
}

IServer * CGameEngine::getServer()
{
	if (server == NULL)
	{
		er.report(LOG_WARNING, _("%s: No server defined, creating local.\n"), AT);
		StartLocalServer();
	}

	return server;
}

void CGameEngine::onChangeTopGui()
{
	switch(getTopGUINo())
	{
	case guiScreen::mainMenu:
		er.report(LOG_INFO, "%s: Menu Main Menu\n", AT);

		// stop network
		StopNetworkServer();
		break;

	case guiScreen::playersMenu:
		er.report(LOG_INFO, "%s: Menu Players\n", AT);

		//StartSingleplayerClient();
		break;

	case guiScreen::gameOptionsMenu:
		er.report(LOG_INFO, "%s: Menu Game Options\n", AT);
		break;

	case guiScreen::settingsMenu:
		er.report(LOG_INFO, "%s: Menu Settings\n", AT);
		break;

	case guiScreen::networkClientMenu:
		er.report(LOG_INFO, "%s: Menu Network Client Options\n", AT);

		//StartSingleplayerClient();
		//netClient->queryServerProtocolVersion();
		break;
	}
}

void CGameEngine::ChangeTopGUI (enum guiScreen top)
{
	er.report(LOG_INFO, _("%s: ChangeTopGUI(%d)\n"), AT, top);
	assert(guis[top] != NULL);

	if(gui == NULL)
	{
		gui = guis[top];
	}

	if (top != guiNo)
	{
		guiNo = top;
		gui = guis[top];
		onChangeTopGui();
	}
}

enum guiScreen CGameEngine::getTopGUINo()
{
	return guiNo;
}

gcn::Widget * CGameEngine::getGuiWidget(const char * id)
{
	return mXmlGui->getWidget(id);
}

gcn::Gui * CGameEngine::getGui()
{
	//er.report(LOG_INFO, _("%s: getGui()\n"), AT);
	assert (gui != NULL);
	return gui;
}

gcn::SDLInput * CGameEngine::getGuiSDLInput()
{
	assert (gcnInput != NULL);
	return gcnInput;
}

void CGameEngine::LoadTopGUI (Uint8 top)
{
	gcn::Widget * widget = NULL;
	gcn::Button * button = NULL;
	gcn::Label  * label = NULL;
	gcn::Icon   * icon = NULL;
	gcn::Image  * image = NULL;

	er.report(LOG_INFO, _("%s: LoadTopGUI(%d)\n"), AT, top);

	ostringstream stm;

	switch (top)
	{
	case 0:
		if ((widget = mXmlGui->getWidget("0-main-menu")) != NULL)
		{
			guis[0]->setTop(widget);

			if( (widget = mXmlGui->getWidget("0-start-game")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Start game"));
			}

			if( (widget = mXmlGui->getWidget("0-start-network-game")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Start network game"));
			}

			if( (widget = mXmlGui->getWidget("0-join-network-game")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Join network game"));
			}

			if( (widget = mXmlGui->getWidget("0-settings")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Settings"));
			}

			if( (widget = mXmlGui->getWidget("0-bomberman")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("About OpenAtomic"));
			}

			if( (widget = mXmlGui->getWidget("0-manual")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Manual"));
			}

			if( (widget = mXmlGui->getWidget("0-exit-bomberman")) != NULL )
			{
				button = static_cast<gcn::Button *> (widget);
				button->setCaption(_("Exit OpenAtomic"));
			}

			if( (widget = mXmlGui->getWidget("0-background")) != NULL )
			{
				icon = static_cast<gcn::Icon *> (widget);

				er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_GUI "background.png" );
				if( (image = gcn::Image::load(PATH_DIR_GUI "background.png") ) != NULL )
					icon->setImage(image);
			}

			if( (widget = mXmlGui->getWidget("0-man")) != NULL )
			{
				icon = static_cast<gcn::Icon *> (widget);

				er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_GUI "nuke_small.png" );
				if( (image = gcn::Image::load(PATH_DIR_GUI "nuke_small.png") ) != NULL )
					icon->setImage(image);
			}

			if( (widget = mXmlGui->getWidget("0-logo")) != NULL )
			{
				icon = static_cast<gcn::Icon *> (widget);

				er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_GUI "logo.png" );
				if( (image = gcn::Image::load(PATH_DIR_GUI "logo.png") ) != NULL )
					icon->setImage(image);
			}

			if( (widget = mXmlGui->getWidget("0-version")) != NULL )
			{
				label = static_cast<gcn::Label *> (widget);
				label->setCaption( VER_STRING " (" __DATE__ "), (c) Miroslav Sobotka" );
			}
		}
		break;
	case 1:
		if ((widget = mXmlGui->getWidget("1-players")) != NULL)
		{
			guis[1]->setTop(widget);

			for(Uint32 i = 0; i < CPLAYERS; i++)
			{
				stm.str("");
				stm << "1-player" << i+1 << "-l";

				if( (widget = mXmlGui->getWidget(stm.str())) != NULL )
				{
					label = static_cast<gcn::Label *> (widget);
					label->setCaption( CGameState::instance()->color2str( (imagecolors)i) );
				}

				stm.str("");
				stm << "1-player" << i+1 << "-dd";
				if ( (widget = mXmlGui->getWidget(stm.str()) ) != NULL)
				{
					gcn::DropDown * dd = static_cast<gcn::DropDown*> (widget);
					XmlListModel * listmodel = new XmlListModel;

					listmodel->items.push_back(_("OFF"));
					listmodel->items.push_back(_("Player 1"));
					listmodel->items.push_back(_("Player 2"));
					listmodel->items.push_back(_("AI"));
					dd->setListModel(listmodel);
				}
			}


			if( (widget = mXmlGui->getWidget("1-background")) != NULL )
			{
				icon = static_cast<gcn::Icon *> (widget);

				er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_RES "GLUE3.PCX" );
				if( (image = gcn::Image::load(PATH_DIR_RES "GLUE3.PCX") ) != NULL )
					icon->setImage(image);
			}

		}
		break;
	case 2:
		if ((widget = mXmlGui->getWidget("2-game-options")) != NULL)
		{
			guis[2]->setTop(widget);
		}

		if( (widget = mXmlGui->getWidget("2-background")) != NULL )
		{
			icon = static_cast<gcn::Icon *> (widget);

			er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_RES "GLUE0.PCX" );
			if( (image = gcn::Image::load(PATH_DIR_RES "GLUE0.PCX") ) != NULL )
				icon->setImage(image);
		}

		if( (widget = mXmlGui->getWidget("2-field-preview")) != NULL )
		{
			icon = static_cast<gcn::Icon *> (widget);

			er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_GUI "field-preview.pcx" );
			if( (image = gcn::Image::load(PATH_DIR_GUI "field-preview.pcx") ) != NULL )
				icon->setImage(image);
		}

		break;
	case 3:
		if ((widget = mXmlGui->getWidget("3-settings")) != NULL)
		{
			guis[3]->setTop(widget);
		}

		if ( ( widget = mXmlGui->getWidget("3-node-tf") ) != NULL )
		{
			gcn::TextField * nodeNameTextField = static_cast<gcn::TextField *> (widget);
			//string s = "zlo";
			//nodeNameTextBox->setText(s);
			er.report(LOG_INFO, "%s: nodeNameTextBox->getText(): %s\n", AT, nodeNameTextField->getText().c_str() );
		}

		if( (widget = mXmlGui->getWidget("3-background")) != NULL )
		{
			icon = static_cast<gcn::Icon *> (widget);

			er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_RES "GLUE6.PCX" );
			if( (image = gcn::Image::load(PATH_DIR_RES "GLUE6.PCX") ) != NULL )
				icon->setImage(image);
		}

		break;
	case 4:
		if ((widget = mXmlGui->getWidget("4-network-game-server")) != NULL)
		{
			guis[4]->setTop(widget);
		}

		for(Uint32 i = 0; i < CPLAYERS; i++)
		{
			stm.str("");
			stm << "4-player" << i+1 << "-l";

			if( (widget = mXmlGui->getWidget(stm.str())) != NULL )
			{
				label = static_cast<gcn::Label *> (widget);
				label->setCaption( CGameState::instance()->color2str( (imagecolors)i) );
			}

			stm.str("");
			stm << "4-player" << i+1 << "-dd";
			if ( (widget = mXmlGui->getWidget(stm.str()) ) != NULL)
			{
				gcn::DropDown * dd = static_cast<gcn::DropDown*> (widget);
				XmlListModel * listmodel = new XmlListModel;

				listmodel->items.push_back(_("OFF"));
				listmodel->items.push_back(_("Player 1"));
				listmodel->items.push_back(_("Player 2"));
				listmodel->items.push_back(_("AI"));
				dd->setListModel(listmodel);
			}
			else
			{
				er.report(LOG_WARNING, _("%s: GUI Widget %s doesn't exists!\n"), AT, "3-scheme-dd" );
			}
		}

		if( (widget = mXmlGui->getWidget("4-background")) != NULL )
		{
			icon = static_cast<gcn::Icon *> (widget);

			er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_RES "GLUE5.PCX" );
			if( (image = gcn::Image::load(PATH_DIR_RES "GLUE5.PCX") ) != NULL )
				icon->setImage(image);
		}

		break;
	case 5:
		if ((widget = mXmlGui->getWidget("5-network-game-client-options")) != NULL)
		{
			guis[5]->setTop(widget);
		}

		if( (widget = mXmlGui->getWidget("5-background")) != NULL )
		{
			icon = static_cast<gcn::Icon *> (widget);

			er.report(LOG_INFO, _("%s: Loading '%s'.\n"), AT, PATH_DIR_RES "GLUE5.PCX" );
			if( (image = gcn::Image::load(PATH_DIR_RES "GLUE5.PCX") ) != NULL )
				icon->setImage(image);
		}

		break;
	default:
		break;
	}
}



void CGameEngine::InvalidateArea (SDL_Rect r)
{
	//er.report(LOG_INFO, _("%s: InvalidateArea(%d, %d, %d, %d)\n"), AT, r.x, r.y, r.w, r.h);
	mUglyRects.push_back(r);
}

void CGameEngine::InvalidateArea (Uint16 x, Uint16 y, Uint16 w, Uint16 h)
{
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	InvalidateArea(r);
}
