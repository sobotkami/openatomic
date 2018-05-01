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

#ifndef CLIENT_H
#define CLIENT_H

#include <network/messages.h>

/**
 * @brief Zakladni trida klienta. Definuje predevsim "myslici" funkci Think().
 */
class IClient : public IMessages
{
public:
	/**
	 * @brief Funkce by mela byt volana pravidelne v herni smycce. Vyzvedava zpravy z fronty zprav, zpracovavava je a pripadne plni frontu zprav.
	 */
	virtual void think() = 0;

	/**
	 * @brief Funkce slouzi na zjisteni stavu, zda je klient na nejaky server pripojen.
	 * @return Pokud je klient pouze lokalni, predpoklada se vraceni TRUE, pokud je pouzit jako sitovy klient, vraci se hodnota promenne isRunning.
	 */
	virtual bool isConnected() = 0;

	/**
	 * @brief Destruktor.
	 */
	virtual ~IClient() {}
};

#endif // CLIENT_H
