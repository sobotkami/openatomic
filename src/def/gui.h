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

#ifndef GUI_H
#define GUI_H

typedef enum {
	off,
	key0,
	key1,
	ai,
	net
} playerStateGui;

enum guiScreen {
	mainMenu = 0,
	playersMenu = 1,
	gameOptionsMenu = 2,
	settingsMenu = 3,
	// 4 was start-network-game
	networkClientMenu = 5,
};

#endif // GUI_H
