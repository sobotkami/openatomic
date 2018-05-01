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

#include <getopt.h>

#include "core/gameengine.h"
#include "game/introstate.h"
#include "debug/debug.h"
#include "def/screen.h"
#include "def/version.h"
#include "def/platform.h"

#include <iostream>
using namespace std;

#ifdef CONF_PF_LINUX 

#include <execinfo.h>
#include <signal.h>

/**
 * @brief Capture signals and print stack trace. Only on GNU/Linux.
 * @param sig Number of the signal that was captured.
 */
void handler (int sig)
{
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n======= Backtrace: =========\n", sig);
    backtrace_symbols_fd(array, size, 2);
    exit(1);
}

#endif

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char *argv[])
{
    // debug
    CErrorReporter er;
    er.setLevel(LOG_INFO);

#ifdef CONF_PF_LINUX 
    signal(SIGSEGV, handler); // install our handler
    signal(SIGABRT, handler); // install our handler
#endif
    // First welcome message
    er.report(LOG_INFO, _("%s: %s version %s\n"), AT, WIN_TITLE, VER_STRING);

#ifdef HAVE_OPENGL
    // Have opengl/glSDL?
    er.report(LOG_INFO, _("%s: Compiled with glSDL wrapper.\n"), AT, WIN_TITLE, VER_STRING);
#endif


    // run flags and parameters
    Uint32 videoflags = SDL_VIDEO_FLAGS;
    while (argc > 1)
    {
        --argc;
        if (strcmp(argv[argc], "-hw") == 0)
        {
            videoflags ^= SDL_HWSURFACE;
        }
        else
            if (strcmp(argv[argc], "-flip") == 0)
        {
            videoflags ^= SDL_DOUBLEBUF;
        }
        else
            if (strcmp(argv[argc], "-fullscreen") == 0)
        {
            videoflags ^= SDL_FULLSCREEN;
        }
        else
            if (strcmp(argv[argc], "-gl") == 0)
        {
            videoflags ^= SDL_GLSDL;
        }
        else
        {
            er.report(LOG_ERROR, _("%s: Usage: %s [-hw] [-flip] [-fast] [-fullscreen]\n"), AT, argv[0]);
            return 1;
        }
    }

    // locales
    setlocale(LC_ALL, "");
    bindtextdomain("openatomic", "./locale");
    textdomain("openatomic");

    // main
    try
    {
        // engine
        //CGameEngine game;

        // initialize the engine
        CGameEngine::instance()->Init(WIN_TITLE, WIN_WIDTH, WIN_HEIGHT, WIN_BPP, videoflags);

        // load the intro
        CGameEngine::instance()->ChangeState(CIntroState::Instance());

        // main loop
        CGameEngine::instance()->Run();

        // cleanup the engine
        CGameEngine::instance()->Cleanup();
    }
    /*
     * Catch all Guichan exceptions
     */
    catch (gcn::Exception e)
    {
        er.report(LOG_ERROR, _("%s: GCN exception: %s\n"), AT, e.getMessage().c_str());
        return 2;
    }
    /*
     * Catch all Std exceptions
     */
    catch (std::exception e)
    {
        er.report(LOG_ERROR, _("%s: STD exception: %s\n"), AT, e.what());
        return 3;
    }
    /*
     * Catch all Unknown exceptions
     */
    catch (...)
    {
        er.report(LOG_ERROR, _("%s: Unknown exception\n"), AT);
        return 4;

    }

    return 0;
}
