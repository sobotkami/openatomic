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

#ifndef IMESSAGES_H
#define IMESSAGES_H

#include <def/gui.h>
#include <entities/network/networkMessage.h>

/**
 * @brief Rozhrani IMessages definuje elementarni zpravy, ktere se budou posilat mezi klienty a serverem. At uz primym volanim, nebo pomoci prenosu pres sit.
 */
class IMessages
{
public:
	/**
	 * @brief Presun hrace z bodu X1,Y1 na bod X2,Y2. Pridej stav hrace a jeho aktualni stav zivota.
	 */
	virtual void player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life) = 0;
	/**
	 * @brief Poloz nebo nech explodovat bombu -- definovano pomoci state.
	 */
	virtual void bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state) = 0;
	/**
	 * @brief Aktualizuj inventar hrace.
	 */
	virtual void inventory(Uint8 player, powerup_t powerup, Sint8 value) = 0;
	/**
	 * @brief Aktualizuj jedno herni pole.
	 */
	virtual void updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup) = 0;
	/**
	 * @brief Nastav vychozi pozici hrace.
	 */
	virtual void setDefPlPos(Uint8 player, Uint8 x, Uint8 y) = 0;
	/**
	 * @brief Zmen menu ve hre (GUI)
	 */
	virtual void changeMenu(enum guiScreen menu) = 0;
	/**
	 * @brief Odstartuj hru.
	 */
	virtual void startGame() = 0;
	/**
	 * @brief Ukaz vysledkovou listinu.
	 */
	virtual void showResults() = 0;
	/**
	 * @brief Ukonci hru.
	 */
	virtual void exitGame() = 0;
	/**
	 * @brief Zaregistruj hrace do hry + jake ovladani bude pouzivat.
	 */
	virtual void registerPlayer(Uint8 player, const char *node, plmantype_t control) = 0;
	/**
	 * @brief Odregistruj hrace ze hry.
	 */
	virtual void unregisterPlayer(Uint8 player) = 0;
};

#endif // IMESSAGES_H
