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

#ifndef SERVER_H
#define SERVER_H

#include <network/messages.h>

/**
 * @brief Zakladni trida serveru. Definuje predevsim "myslici" funkci Think() a pridava funkce pro spusteni a zastaveni site.
 */
class IServer : public IMessages
{
public:
	/**
	 * @brief Funkce by mela byt volana pravidelne v herni smycce. Vyzvedava zpravy z fronty zprav, zpracovavava je a pripadne plni frontu zprav.
	 */
	virtual void think() = 0;

	/**
	 * @brief Startuje podporu pro sit. Po spusteni je dostupna na vychozim portu (definovan v CNetwork) na zvolene IP/hostname. Ve vychozim stavu je hostname zvolen jako localhost.
	 * @param hostname IP nebo hostname, kde bude naslouchat program pozadavky od klientu
	 */
	virtual void startNetwork(const char * hostname = "localhost") = 0;
	/**
	 * @brief Vypne podporu site. Klient tak bude prijimat zpravy pouze lokalne.
	 */
	virtual void stopNetwork() = 0;
	/**
	 * @brief Funkce vraci stav site, zda bezi.
	 * @return Vraci TRUE, pokud je zapnuta podpora site. Jinak vraci FALSE.
	 */
	virtual bool isNetworkRunning() = 0;

	/**
	 * @brief Destruktor.
	 */
	virtual ~IServer() {}
};

#endif // SERVER_H
