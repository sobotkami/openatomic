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

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <guichan.hpp>
#include <guichan/sdl.hpp>

#include <def/sdl.h>
#include <3rdparty/xmlgui/xmlgui.h>
#include <debug/errorreporter.h>
#include <global.h>

// network
//#include <network/networkserver.h>
//#include <network/networkclient.h>

#include <def/gui.h>
#include <network/client.h>
#include <network/server.h>

#include <vector>

using namespace std;

class CProgramState;
class CButtonListener;

class CGameEngine {
public:

    static CGameEngine *instance() {
        if (!s_instance)
            s_instance = new CGameEngine;
        return s_instance;
    }

    void Init(const char* title, int width = 640, int height = 480,
              int bpp = 16, Uint32 sdlFlags = 0);
    void Cleanup();

    void ChangeState(CProgramState* state);
    void PushState(CProgramState* state);
    void PopState();

    void HandleEvents();
    void HandleNetwork();
    void Update();
    void Draw();
    void Run();

    bool Running() {
        return mRunning;
    }

    void Quit() {
        mRunning = false;
    }

    void ChangeTopGUI(enum guiScreen top);
    enum guiScreen getTopGUINo();


    gcn::Widget * getGuiWidget(const char * id);
    gcn::Gui * getGui();
    gcn::SDLInput * getGuiSDLInput();
    
    void InvalidateArea(SDL_Rect r);
    void InvalidateArea(Uint16 x, Uint16 y, Uint16 w, Uint16 h);
    vector<SDL_Rect> & getUglyRects()
    {
        return mUglyRects;
    }

    /* stop network client */
    void StopClient();
    /* start network client */
    void StartNetworkClient(const char * hostname);
    /* start local client */
    void StartLocalClient();

    /* stop network server */
    void StopServer();
    /* start network server */
    void StartNetworkServer();
    /* stop network server*/
    void StopNetworkServer();
    /* start local server */
    void StartLocalServer();


    //CNetworkClient * getNetClient();
    IClient * getClient();
    IServer * getServer();


    // Events
    void onChangeTopGui();

private:
    CGameEngine()
    {
        // log everything
        er.setLevel(LOG_INFO);
        er.report(LOG_INFO, _("%s: CGameEngine()\n"), AT);

        gui = NULL;
        guiNo = guiScreen::mainMenu;
        gcnInput = NULL;

        client = NULL;
        server = NULL;
    }

    static CGameEngine *s_instance;

    // the stack of states
    vector<CProgramState*> mStates;
    
    // the stack of invalidate areas
    vector<SDL_Rect> mUglyRects;

    // main game state
    bool mRunning;

    CErrorReporter er;

    /*
     * SDL Screen
     */
    SDL_Surface* mScreen; // Main screen
    Uint32 FastestFlags(Uint32 flags, unsigned width, unsigned height, unsigned bpp);
    bool mFullscreen;

    /*
     * Guichan
     */

    enum guiScreen guiNo;
    gcn::Gui * guis[CGUISTATES];
    gcn::Gui * gui; // A Gui object - binds it all together

    /*
     * Guichan SDL stuff we need
     */
    gcn::SDLGraphics* mGraphics; // Graphics driver
    gcn::SDLImageLoader* mImageLoader; // For loading images
    gcn::SDLInput* gcnInput; // Input driver

    /*
     * Guichan stuff we need
     */
    gcn::ImageFont* mFont; // A font
    CButtonListener* mButtonActionListener;


    /*
     * XML GUI
     */
    XmlGui *mXmlGui; // XmlGui stuff

    void InitButtons();
    void InitButton(const char * id);
    void LoadTopGUI(Uint8 top);

    /*
     * Network
     */
    IClient * client;
    IServer * server;

};

#endif
