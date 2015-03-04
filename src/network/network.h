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

#ifndef NETWORK_H
#define NETWORK_H


#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <network/networkMonitor.h>
#include <network/networkThread.h>
#include <network/networkConnection.h>

#include <def/network.h>
#include <debug/errorreporter.h>

#include <list>

template<typename P>
/**
 * @brief Trida CNetwork. Parametr P udava tridu, ktera bude pouzita jako typ zpravy v cele sitove komunikaci.
 */
class CNetwork
{
public:
    /**
     * @brief Konstruktor.
     * @param server Pokud je promenna TRUE, pak bude instance tridy pracovat jako server. Pokud je FALSE, instance se bude chovat jako klient.
     * @param strIP IP nebo hostname, kde bude server naslouchat, nebo kam se bude klient pripojovat.
     * @param port Cislo portu, na kterem bude server naslouchat, nebo kam se bude klient pripojovat.
     */
    CNetwork(bool server = true, const char * strIP = "localhost", Uint16 port = NET_DEFAULT_PORT )
    {
        er.setLevel(LOG_INFO);

        this->m_pMonitor = new CNetworkMonitor<P>();

        char * ipString = (char *)strIP;
        IPaddress ip;
        this->server = server;

        running = false;
        tcp_sd = NULL;

        if(SDLNet_Init()==-1)
        {
            er.report(LOG_ERROR, _("%s: SDLNet_Init: %s\n"), AT, SDLNet_GetError());
            exit(2);
        }

        if(server)
        {
            ipString = NULL;
        }

        int sockNbr = 0;
        m_pAcceptThread = NULL;
        m_pSendThread = NULL;
        m_pReceiveThread = NULL;


        if (SDLNet_ResolveHost(&ip, ipString, port) < 0)
        {
            er.report(LOG_ERROR, _("%s: SDLNet_ResolveHost: %s\n"), AT, SDLNet_GetError());
        }
        else
        {

            m_pConnection = new CNetworkConnection();

            if (!(tcp_sd = SDLNet_TCP_Open(&ip))) // TCP
            {
                er.report(LOG_ERROR, _("%s: SDLNet_TCP_Open: %s\n"), AT, SDLNet_GetError());
            }
            else
            {

                sockNbr = m_pConnection->addSocket(tcp_sd);

                if (server)
                {
                    er.report(LOG_INFO, _("%s: Server started\n"), AT );

                    m_pAcceptThread = new CAcceptThread<P>(m_pMonitor, m_pConnection, 1, tcp_sd);
                    m_pAcceptThread->start();

                    m_pSendThread = new CSendThread<P>(m_pMonitor, m_pConnection);
                    m_pSendThread->start();
                }
                else
                {
                    er.report(LOG_INFO, _("%s: Client started\n"), AT );

                    m_pReceiveThread = new CReceiveThread<P>(m_pMonitor, m_pConnection, sockNbr);
                    m_pReceiveThread->start();

                    m_pSendThread = new CSendThread<P>(m_pMonitor, m_pConnection);
                    m_pSendThread->start();
                }

                running = true;
            }
        }
    }

    /**
     * @brief Metoda na zjisteni, zda na dane kombinaci hostname:port bezi server.
     * @param hostname Hostname ciloveho pocitace.
     * @param port Port ciloveho pocitace.
     * @return Vraci TRUE, pokud na zadane kombinaci hostname:port bezi server, FALSE v opacnem pripade.
     */
    static bool serverExists(const char * hostname, Uint16 port = NET_DEFAULT_PORT)
    {
        IPaddress ip;
        TCPsocket sock;
        bool exists = false;

        if (SDLNet_ResolveHost(&ip, hostname, port) < 0)
        {
            // could not resolve host
            throw _("Hostname can not be resolved!");
        }
        else
        {
            if (!(sock = SDLNet_TCP_Open(&ip))) // TCP
            {
                // server dont running
                throw SDLNet_GetError();
            }
            else
            {
                // all OK
                SDLNet_TCP_Close(sock);
                exists = true;
            }
        }

        return exists;
    }

    /**
     * @brief Destruktor.
     */
    ~CNetwork()
    {
        er.report(LOG_INFO, _("%s: Closing network\n"), AT );

        if (server)
        {
            m_pAcceptThread->stop();
            delete m_pAcceptThread;
        }
        else
        {
            m_pReceiveThread->stop();
            delete m_pReceiveThread;
        }

        if (m_pSendThread != NULL)
        {
            m_pSendThread->stop();
            delete m_pSendThread;
            m_pSendThread = NULL;
        }

        delete m_pConnection;

        SDLNet_Quit();

        delete m_pMonitor;
    }

    /**
     * @brief Odesle zpravu.
     * @param i Zprava.
     */
    void send(P i)
    {
        m_pMonitor->insertOutcoming(i);
    }

    /**
     * @brief Prijme zpravu.
     * @param i Zprava.
     * @return Vraci TRUE, pokud nejakou zpravu vyzvedl z fronty, FALSE pokud byla fronta prazdna.
     */
    bool get(P &i)
    {
        return m_pMonitor->getIncoming(&i);
    }

    /**
     * @brief Funkce na kontrolu stavu site.
     * @return Vraci TRUE, pokud je spojeni v poradku, FALSE v opacnem pripade.
     */
    bool isRunning()
    {
        if(server)
        {
            return (running && m_pAcceptThread->isRunning() && m_pSendThread->isRunning());
        }
        else
        {
            return (running && m_pReceiveThread->isRunning() && m_pSendThread->isRunning());
        }
    }

protected:

    CReceiveThread<P>  * m_pReceiveThread;
    CSendThread<P>     * m_pSendThread;
    CAcceptThread<P>   * m_pAcceptThread;


    CNetworkMonitor<P> * m_pMonitor;
    CNetworkConnection * m_pConnection;

    TCPsocket tcp_sd;

    CErrorReporter er;
private:

    bool server;
    bool running;

};




#endif // NETWORK_H
