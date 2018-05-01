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

#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "../core/go_player.h"

#include <vector>
#include "../def/point.h"

using namespace std;

class CAIPlayer : public CGO_Player
{
private:
	imagecolors oponent;

public:
	CAIPlayer();
	void setOponent(imagecolors color);
	imagecolors getOponent();
	void removeOponent();
};

#endif // AIPLAYER_H
