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

#ifndef DEBUGNETWORK_H
#define DEBUGNETWORK_H

#include <entities/network/networkMessage.h>

/**
 * @brief Trida slouzici pro ladeni protokolu.
 */
class CDebugNetwork
{
public:	
	/**
	 * @brief Trida tiskne obsah zpravy predany parametrem s prefixem.
	 * @param pre Prefix, ktery je tisknut pred vlastni obsah zpravy
	 * @param m Zprava, ktera se bude parsovat a jeji obsah bude vytisknut na standardni vystup.
	 */
	static void printMessagePayload( const char * pre, msg_t &m );
};

#endif // DEBUGNETWORK_H
