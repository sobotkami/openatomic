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

#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <debug/errorreporter.h>

#include <map>

/**
 * @brief Trida CNetworkConnection
 */
class CNetworkConnection
{
public:
    /**
     * @brief Kontruktor. Nastavuje na vychozi hodnotu promenne.
     */

    CNetworkConnection()
    {
        er.setLevel(LOG_INFO);

        m_nbr = 0;
        m_iteratorPos = 0;
        m_socketList = std::map<int,TCPsocket>();

        m_pMutex = SDL_CreateMutex();
    }

    /**
     * @brief Destruktor.
     */
    ~CNetworkConnection()
    {
        SDL_DestroyMutex(m_pMutex);
    }

    /**
     * @brief Pridani socketu do seznamu, kde je udrzujeme.
     * @param sock Cislo socketu.
     * @return Vraci pocet aktualne pripojenych klientu.
     */
    int addSocket(TCPsocket sock)
    {
        IPaddress *ipaddr = SDLNet_TCP_GetPeerAddress(sock); // TCP!
        Uint32 ip = 0;
        const char *host = NULL;

        if(ipaddr) // Only for IPv4!
        {
            ip = SDL_SwapBE32(ipaddr->host);
            host = SDLNet_ResolveIP(ipaddr);

            er.report(LOG_INFO, _("%s: New client: %s %d.%d.%d.%d:%d\n"), AT,
                      host ? host : "",
                      ip>>24, (ip>>16)&0xff, (ip>>8)&0xff, ip&0xff,
                      (Uint32)ipaddr->port);
        }

        SDL_LockMutex(m_pMutex);
        m_socketList[m_nbr] = sock;
        int nbr = m_nbr;
        m_nbr++;
        SDL_UnlockMutex(m_pMutex);
        return nbr;
    }

    /**
     * @brief Odebrani socketu
     * @param nbr Cislo odebiraneho socketu.
     */

    void disconnectSocket(int nbr)
    {
        SDL_LockMutex(m_pMutex);
        TCPsocket sock = m_socketList[nbr];
        m_socketList.erase(nbr);
        if (sock != NULL)
        {
            SDLNet_TCP_Close(sock); // TCP!
            er.report(LOG_WARNING, _("%s: Socket Closed by NetworkConnection\n"), AT );
        }
        SDL_UnlockMutex(m_pMutex);
    }

    /**
     * @brief Pomocna funkce na prochazeni pole. Nastavi iterator na zacatek. Zamkne se zamek, aby nam s polem behem iterace nikdo nic nedelal.
     */
    void beginIteration()
    {
        SDL_LockMutex(m_pMutex);
        m_iteratorPos = 0;
    }

    /**
     * @brief Konec iterovani. Slouzi pouze na odemceni zamku, aby s polem zase mohl nekdo pracovat.
     */
    void endIteration()
    {
        SDL_UnlockMutex(m_pMutex);
    }

    /**
     * @brief Pomocna funkce na iteraci. Vraci nasledujici platny prvek.
     * @return
     */
    TCPsocket* next()
    {
        while (m_iteratorPos < m_nbr && m_socketList[m_iteratorPos] == NULL)
            m_iteratorPos++;
        if (m_iteratorPos < m_nbr)
        {
            m_iteratorPos++;
            return &m_socketList[m_iteratorPos-1];
        }
        return NULL;
    }

    /**
     * @brief Funkce, ktera na zaklade cisla socketu vraci TCP socket.
     * @param nbr Cislo socketu, podle ktereho hledame TCP socket.
     * @return Vraci TCP socket z pole, kde je identifikovan cislem socketu.
     */
    TCPsocket* getSocket(int nbr)
    {
        return &m_socketList[nbr];
    }


protected:
    /**
     * Mapa zavislosti cisla socketu na TCP socket.
     */
    std::map<int,TCPsocket> m_socketList;
    /**
     * Aktualni pocet pripojenych klientu.
     */
    int m_nbr;
    /**
     * Cislo polozky pri iteraci.
     */
    int m_iteratorPos;

    /**
     * Zamek na hlidani pristupu do pole.
     */
    SDL_mutex* m_pMutex;

    /**
     * Chybovy reporter, ktery vypisuje hlasky na stadardni vystupy.
     */
    CErrorReporter er;

};



#endif // NETWORKCONNECTION_H
