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
#include <core/gameengine.h>
#include <game/programstate.h>
#include <game/menustate.h>
#include <audio/playsound.h>
#include <3rdparty/xmlgui/xmlgui.h>


#include <iterator>
#include <vector>
#include <algorithm>
// boost filesystem
#include <boost/filesystem.hpp>
// boost regex
#include <boost/regex.hpp>

// object instance
CMenuState CMenuState::m_MenuState;

void CMenuState::Init ()
{
    er.setLevel(LOG_INFO);

    CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);

    redraw = true;

    loadSchemeFiles();

    //gcn::Widget * w = CGameEngine::instance()->getGuiWidget("3-scheme-dd");

    er.report(LOG_INFO, _("%s: CMenuState Init\n"), AT);
}

void CMenuState::Cleanup ()
{
    er.report(LOG_INFO, _("%s: CMenuState Cleanup\n"), AT);
}

void CMenuState::Pause ()
{
    er.report(LOG_INFO, _("%s: CMenuState Pause\n"), AT);
}

void CMenuState::Resume ()
{
    er.report(LOG_INFO, _("%s: CMenuState Resume\n"), AT);
}

void CMenuState::HandleEvents ()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            CGameEngine::instance()->Quit();
            //m_game->Quit();
            break;

        case SDL_KEYDOWN:
            redraw = true;
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                if(CGameEngine::instance()->getTopGUINo() == 0)
                {
                    CGameEngine::instance()->Quit();
                }
                else
                {
                    CGameEngine::instance()->ChangeTopGUI(guiScreen::mainMenu);
                }
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            redraw = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            redraw = true;
            break;
        case SDL_MOUSEBUTTONUP:
            redraw = true;
            CPlaySound::instance()->PlayMenuClick();
            break;
        default:
            break;
        }
        CGameEngine::instance()->getGuiSDLInput()->pushInput(event);
        //m_game->gcnInput->pushInput(event); // GUI
    }

    /*
    CGameEngine::instance()->getClient()->think();
    CGameEngine::instance()->getServer()->think();
    */

    /*
    // TEST?
    if( CGameEngine::instance()->getClient() != NULL )
    {
        CGameEngine::instance()->getClient()->think();

        ///
        if( !CGameEngine::instance()->getNetClient()->isRunning())
        {
            er.report(LOG_WARNING, _("%s: Client is not running -- stop.\n"));
            CGameEngine::instance()->StopNetClient();
        }
        else
        {
            CGameEngine::instance()->getNetClient()->Think();
        }
        ///
    }
    else
    {
        CGameEngine::instance()->StartSingleplayerClient();
    }
    */
}

void CMenuState::Update ()
{
    try
    {
        CGameEngine::instance()->getGui()->logic();
        //m_game->gui->logic();
    }
    catch (gcn::Exception e)
    {
        er.report(LOG_ERROR, _("%s: GCN exception: %s\n"), AT, e.getMessage().c_str());
    }
}

void CMenuState::Draw ()
{
    if (redraw)
    {
        redraw = false;
        // Draw the gui
        CGameEngine::instance()->getGui()->draw();
        //m_game->gui->draw();
        CGameEngine::instance()->InvalidateArea(0,0,0,0);
        //m_game->InvalidateArea(0, 0, 0, 0);
    }

    // Update the screen
    if ((SDL_GetVideoSurface()->flags & SDL_DOUBLEBUF) == SDL_DOUBLEBUF)
    {
        SDL_Flip(SDL_GetVideoSurface());
        CGameEngine::instance()->getUglyRects().clear();
        //m_game->getUglyRects().clear();
    }
    else
    {

        std::vector<SDL_Rect>::size_type sz = CGameEngine::instance()->getUglyRects().size();
        //std::vector<SDL_Rect>::size_type sz = m_game->getUglyRects().size();
        SDL_Rect r;

        for (Uint32 i = 0; i < sz; i++)
        {

            r = CGameEngine::instance()->getUglyRects().back();
            //r = m_game->getUglyRects().back();
            SDL_UpdateRect(SDL_GetVideoSurface(), r.x, r.y, r.w, r.h);
            CGameEngine::instance()->getUglyRects().pop_back();
            //m_game->getUglyRects().pop_back();
        }
    }

    SDL_Delay(1);
}

void CMenuState::loadSchemeFiles()
{
    boost::filesystem::path p (PATH_DIR_SCHEMES);
    boost::regex re;
    boost::smatch what;
    string mystring;

    try
    {
        // Set up the regular expression for case-insensitivity
        re.assign(".*[\\/](.*\\.sch)$", boost::regex_constants::icase);
    }
    catch (boost::regex_error& e)
    {
        er.report(LOG_ERROR, _("%s: '%s' exists, but is neither a regular file nor a directory!\n"), AT, ".*\\.sch", e.what() );
        return;
    }

    try
    {
        if (exists(p))    // does p actually exist?
        {
            if (boost::filesystem::is_regular_file(p))
            { // is p a regular file?
                er.report(LOG_ERROR, _("%s: %s is not a directory!\n"), AT, PATH_DIR_SCHEMES);
                return;
            }
            else if (is_directory(p))
            { // is p a directory?

                // store paths,
                typedef std::vector<boost::filesystem::path> vec;
                // so we can sort them later
                vec v;

                copy(boost::filesystem::directory_iterator(p), boost::filesystem::directory_iterator(), back_inserter(v));

                // sort, since directory iteration
                // is not ordered on some file systems
                sort(v.begin(), v.end());


                gcn::Widget * w = CGameEngine::instance()->getGuiWidget("3-scheme-dd");
                //gcn::Widget * w = m_game->getGuiWidget("3-scheme-dd");
                if (w != NULL)
                {
                    gcn::DropDown * dd = static_cast<gcn::DropDown*> (w);
                    XmlListModel * listmodel = new XmlListModel;

                    for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
                    {
                        mystring.assign( (*it).c_str() );
                        std::string::const_iterator start = mystring.begin();
                        std::string::const_iterator end   = mystring.end();

                        while (boost::regex_search(start, end, what, re))
                        {
                            string stest(what[1].first, what[1].second);
                            listmodel->items.push_back(stest);

                            // Update the beginning of the range to the character
                            // following the whole match
                            start = what[0].second;
                        }
                    }
                    dd->setListModel(listmodel);

                    // TODO!!
                    //dd->setSelected(10); get .sch file from configure and select his.
                }
                else
                {
                    er.report(LOG_WARNING, _("%s: GUI Widget %s doesn't exists!\n"), AT, "3-scheme-dd" );
                }


            }
            else
            {
                er.report(LOG_ERROR, _("%s: %s exists, but is neither a regular file nor a directory!\n"), AT, PATH_DIR_SCHEMES);
                return ;
            }
        }
        else
        {
            er.report(LOG_ERROR, _("%s: %s does not exist!\n"), AT, PATH_DIR_SCHEMES);
            return;
        }
    }
    catch (const boost::filesystem::filesystem_error& ex)
    {
        er.report(LOG_ERROR, _("%s: loadSchemeFiles(): %s\n"), AT, ex.what());
        return;
    }

}
