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

#ifndef CLOCALSERVER_H
#define CLOCALSERVER_H

#include <network/server.h>
#include <network/network.h>

#include <debug/errorreporter.h>

/**
 * @brief Trida lokalniho serveru.
 */
class CLocalServer : public IServer
{
public:
	/**
	 * @brief Kontruktor.
	 */
	CLocalServer();

	/**
	 * @brief Implementace vsech metod rozhrani IMessages.
	 */
	virtual void player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life);
	virtual void bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state);
	virtual void inventory(Uint8 player, powerup_t powerup, Sint8 value);
	virtual void updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup);
	virtual void setDefPlPos(Uint8 player, Uint8 x, Uint8 y);
	virtual void changeMenu(enum guiScreen menu);
	virtual void startGame();
	virtual void showResults();
	virtual void exitGame();
	virtual void registerPlayer(Uint8 player, const char *node, plmantype_t control);
	virtual void unregisterPlayer(Uint8 player);

	/**
	 * @brief Implementace vsech metod rozhrani IClient.
	 */
	virtual void think();

	/* @briefMetody na testovani, zapinani a vypinani podpory prijimani zprav pres sit */
	virtual void startNetwork(const char * hostname = "localhost");
	virtual void stopNetwork();
	virtual bool isNetworkRunning();

private:
	/**
	 * @brief Privatni promenna, ktera se inicializuje spustenim site.
	 */
	CNetwork<msg_t> *network;

	/**
	 * @brief Trida slouzici k vypisu zprav na ladeni.
	 */
	CErrorReporter er;
};

#endif // CLOCALSERVER_H
