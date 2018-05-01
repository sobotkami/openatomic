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

#ifndef BUTTONLISTENER_H
#define BUTTONLISTENER_H

#include <game/playstate.h>

#include <core/gameengine.h>
#include <core/gameresults.h>
#include <core/gamestate.h>
#include <network/network.h>

#include <entities/network/networkMessage.h>

class CButtonListener : public gcn::ActionListener {
public:

	CButtonListener(CGameEngine * game) : m_game(game) {
	}

	void action(const gcn::ActionEvent& actionEvent) {

		//std::cout << actionEvent.getId() << std::endl;

		// "0-main-menu"
		if (actionEvent.getId() == "0-start-game") {
			// stop network server
			m_game->StopNetworkServer();
			// start local client
			m_game->StartLocalClient();

			m_game->ChangeTopGUI(guiScreen::playersMenu);
		}
		if (actionEvent.getId() == "0-start-network-game") {
			// starting network game
			m_game->StartNetworkServer();

			// start network client
			m_game->StartNetworkClient("localhost");

			m_game->ChangeTopGUI(guiScreen::playersMenu); // was 4
		}
		if (actionEvent.getId() == "0-join-network-game") {
			m_game->ChangeTopGUI(guiScreen::networkClientMenu);
		}
		if (actionEvent.getId() == "0-settings") {
			m_game->ChangeTopGUI(guiScreen::settingsMenu);
		}
		if (actionEvent.getId() == "0-bomberman") {
			std::cout << "[stuff] About Open Atomic" << std::endl;
		}
		if (actionEvent.getId() == "0-manual") {
			std::cout << "[stuff] Online manual" << std::endl;
		}
		if (actionEvent.getId() == "0-exit-bomberman") {
			m_game->Quit();
		}

		// "1-players"
		if (actionEvent.getId() == "1-back") {
			m_game->ChangeTopGUI(guiScreen::mainMenu);
		}

		if (actionEvent.getId() == "1-next") {

			Uint8 active = 0;

			// DEBUG: print info about players
			std::cout << "Registered players\n" <<
						 "------------------" << endl;
			for(Uint8 i = 0; i < CPLAYERS; i++)
			{
				if (CGameState::instance()->getPlayer(i).control != typeNone)
				{
					active++;
				}

				std::cout << "P(" << (int)i << ") node: " <<  CGameState::instance()->getPlayer(i).node
						  << ", control: " << CGameState::instance()->getPlayer(i).control
						  << endl;
			}


			// we need minimal 2 players
			if(active < 2)
			{
				// TODO: to GUI
				std::cout << "Ich brauche mindestens 2 Spieler!" << endl;
			}
			else
			{
				// network client exist and we arent server, then stop.
				if (CGameState::instance()->getNetworkGame() && !m_game->getServer()->isNetworkRunning() )
				{
					std::cout << "We are waiting for a server response ..." << std::endl;
				}
				else
				{
					m_game->ChangeTopGUI(guiScreen::gameOptionsMenu);
				}
			}

		}

		if (actionEvent.getId() == "1-player1-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 0);
		}

		if (actionEvent.getId() == "1-player2-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 1);
		}

		if (actionEvent.getId() == "1-player3-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 2);
		}

		if (actionEvent.getId() == "1-player4-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 3);
		}

		if (actionEvent.getId() == "1-player5-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 4);
		}

		if (actionEvent.getId() == "1-player6-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 5);
		}

		if (actionEvent.getId() == "1-player7-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 6);
		}

		if (actionEvent.getId() == "1-player8-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 7);
		}

		if (actionEvent.getId() == "1-player9-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 8);
		}

		if (actionEvent.getId() == "1-player10-dd") {
			updatePlayerGuiState(actionEvent.getSource(), 9);
		}

		// "2-game-options"
		if (actionEvent.getId() == "2-back") {
			m_game->ChangeTopGUI(guiScreen::playersMenu);
		}
		if (actionEvent.getId() == "2-next") {
			CGameResults::instance()->ResetResults(); // TODO: thgrouth network!!

			//m_game->ChangeState(CPlayState::Instance());

			//if (m_game->getClient() != NULL)
			m_game->getClient()->startGame();
		}

		// "3-settings"
		if (actionEvent.getId() == "3-back") {
			m_game->ChangeTopGUI(guiScreen::mainMenu);
		}

		if (actionEvent.getId() == "3-scheme-dd") {
			gcn::DropDown* dd = static_cast<gcn::DropDown*> (actionEvent.getSource());
			std::cout << "Schema je nyni: " << dd->getListModel()->getElementAt(dd->getSelected()) << std::endl;
		}

		// "4-network-game-server"
		if (actionEvent.getId() == "4-back") {
			m_game->ChangeTopGUI(guiScreen::mainMenu);
		}

		// "5-network-game-client-options"
		if (actionEvent.getId() == "5-back") {
			m_game->ChangeTopGUI(guiScreen::mainMenu);
		}

		if (actionEvent.getId() == "5-game-server-b") {
			//m_game->StartMultiplayerClient(((gcn::TextField *)(m_game->getGuiWidget("5-game-server-tf")))->getText().c_str());
			//std::cout << "[stuff] isRunning() = " <<  m_game->getNetClient()->isRunning() << std::endl;


			try
			{
				std::cout
						<< "CONNECTION: "
						<< CNetwork<msg_t>::serverExists(((gcn::TextField *)(m_game->getGuiWidget("5-game-server-tf")))->getText().c_str())
						<< std::endl;

				// join to game
				CGameEngine::instance()->StartNetworkClient(((gcn::TextField *)(m_game->getGuiWidget("5-game-server-tf")))->getText().c_str());

				while ( !CGameEngine::instance()->getClient()->isConnected() )
				{ // wait for success connection
					std::cout << "Connection not yet established .. waiting 500 ms." << std::endl;
					SDL_Delay(500);
				}

				// set up players
				m_game->ChangeTopGUI(guiScreen::playersMenu);
			}
			catch( char * e )
			{
				std::cout << "Error: " << e << std::endl;
			}

		}
	}
private:
	CGameEngine * m_game;

	void updatePlayerGuiState(gcn::Widget* w, int p)
	{
		gcn::DropDown* dd = static_cast<gcn::DropDown*> (w);
		/*
		CGameState::instance()->setPlayerGuiState
				(
					CGameState::instance()->str2playerStateGui(
						dd->getListModel()->getElementAt(dd->getSelected())),
					p );
					*/
		CGameState::instance()->setPlayerGuiState((plmantype_t)dd->getSelected(), p);

	}
};

#endif
