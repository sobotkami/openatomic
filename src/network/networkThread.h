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

#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H


#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <network/networkMonitor.h>
#include <network/networkConnection.h>

#include <debug/errorreporter.h>

#include <list>

/**
 * @brief Trida CNetworkThread. Slouzi jako zaklad pro dalsi oddedene tridy.
 */
template<typename T>
class CNetworkThread
{
public:
    /**
     * @brief Konstruktor nuluje a pripravuje promenne.
     * @param net Monitoru zmen.
     * @param conn Spojeni s klientem.
     */
    CNetworkThread(CNetworkMonitor<T> * net, CNetworkConnection * conn)
    {
        er.setLevel(LOG_INFO);

        m_running = false;
        m_pThread = NULL;
        m_stopMutex = SDL_CreateMutex();

        m_pMonitor = net;
        m_pConnection = conn;
    }

    /**
     * @brief Destruktor.
     */
    virtual ~CNetworkThread()
    {
        SDL_DestroyMutex(m_stopMutex);
    }

    /**
     * @brief Vytvari a spusti vlakno
     */
    void start()
    {
        if (m_running)
            return;

        m_running = true;
        m_pThread = SDL_CreateThread(staticWorker, this);
    }

    /**
     * @brief Virtualni trida run v zakladni nedela nic. Jeji obsah definuji tridy oddedene od teto.
     */
    virtual void run()
    {
        // nic
    }

    /**
     * @brief Funkce vraci stav vlakna.
     * @return Vraci TRUE, pokud je spusteno vlakno.
     */
    bool isRunning()
    {
        return m_running;
    }

protected:
    bool m_running;
    SDL_Thread* m_pThread;
    SDL_mutex* m_stopMutex;
    CNetworkMonitor<T> * m_pMonitor;
    CNetworkConnection * m_pConnection;

    CErrorReporter er;


private:
    static int staticWorker(void *p) {
        ((CNetworkThread<T>*) p)->run();
        return 0;
    }
};


/**
 * @brief Trida CReceiveThread. Slouzi jako ctenar zprav ze site.
 */
template<typename T>
class CReceiveThread : public CNetworkThread<T>
{
public:
    /**
     * @brief Kontruktor.
     * @param net Monitor zmen.
     * @param conn Spojeni s klientem.
     * @param sockNbr Cislo socketu, kde se bude cist.
     */
    CReceiveThread(CNetworkMonitor<T> * net, CNetworkConnection * conn, int sockNbr) : CNetworkThread<T>(net,conn)
    {
        m_sockNbr = sockNbr;
    }

    /**
     * @brief Destruktor.
     */
    ~CReceiveThread()
    {
        // nic
    }

    /**
     * @brief Zastavi vlakno cteni.
     */
    void stop()
    {
        SDL_LockMutex(this->m_stopMutex);

        if (this->m_pThread == NULL)
        {
            this->er.report(LOG_WARNING, _("%s: ReceiverThread allready stopped\n"), AT );
            SDL_UnlockMutex(this->m_stopMutex);
            return;
        }

        if (this->m_running)
        {
            this->m_running = false;
            SDL_KillThread(this->m_pThread);
            this->er.report(LOG_WARNING, _("%s: ReceiverThread killed\n"), AT );
        }

        this->m_pThread = NULL;
        this->m_pConnection->disconnectSocket(this->m_sockNbr);

        this->er.report(LOG_INFO, _("%s: ReceiverThread closed\n"), AT );
        SDL_UnlockMutex(this->m_stopMutex);
    }

    /**
     * @brief Spousti vlakno cteni.
     */
    void run()
    {
        T buffer;
        TCPsocket* sock = NULL;

        while (this->m_running)
        {
            sock = this->m_pConnection->getSocket(this->m_sockNbr);
            if (sock != NULL && SDLNet_TCP_Recv(*sock, &buffer, sizeof(buffer)) > 0)
            {
                this->m_pMonitor->insertIncoming(buffer);
            }
            else
            {
                this->er.report(LOG_WARNING, _("%s: ReceiverThread - socket disconnected\n"), AT );
                this->m_running = false;
                break;
            }

        }
        stop();
    }

protected:
    int m_sockNbr;
};

/**
 * @brief Trida CSendThread. Slouzi k posilani zprav pres sit.
 */
template<typename T>
class CSendThread : public CNetworkThread<T>
{
public:
    /**
     * @brief Kontruktor. 
     * @param net Monitor zmen.
     * @param conn Spojeni s klientem.
     */
    CSendThread(CNetworkMonitor<T> * net, CNetworkConnection * conn) : CNetworkThread<T>(net,conn)
    {

    }

    /**
     * @brief Destruktor.
     */
    ~CSendThread()
    {
        // nic
    }

    /**
     * @brief Zastavi vlakno zapisu.
     */
    void stop()
    {
        SDL_LockMutex(this->m_stopMutex);
        if (this->m_pThread == NULL)
        {
            this->er.report(LOG_WARNING, _("%s: SendThread allready stopped\n"), AT );
            SDL_UnlockMutex(this->m_stopMutex);
            return;
        }


        if (this->m_running)
        {
            this->m_running = false;
            SDL_KillThread(this->m_pThread);
            this->er.report(LOG_WARNING, _("%s: SendThread killed\n"), AT );
        }

        this->m_pThread = NULL;

        this->er.report(LOG_INFO, _("%s: SendThread closed\n"), AT );

        SDL_UnlockMutex(this->m_stopMutex);
    }

    /**
     * @brief Spousti vlakno zapisu.
     */
    void run()
    {
        while (this->m_running)
        {
            T buffer;
            this->m_pMonitor->getOutcoming(&buffer);

            this->m_pConnection->beginIteration();
            TCPsocket* sock = this->m_pConnection->next();
            while(sock != NULL)
            {
                SDLNet_TCP_Send(*sock, &buffer, sizeof(T));
                sock = this->m_pConnection->next();
            }
            this->m_pConnection->endIteration();
        }
        stop();
    }

protected:

};


/**
 * @brief Trida CAcceptThread. Prijima pozadavky od novych klientu.
 */
template<typename T>
class CAcceptThread : public CNetworkThread<T>
{
public:

    /**
     * @brief Kontruktor.
     * @param net Monitor zmen.
     * @param conn Spojeni s klienty.
     * @param nbr Maximalni pocet klientu, kteri se mohou pripojit.
     * @param sock Socket, kde se bude cekat na prichozi spojeni.
     */
    CAcceptThread(CNetworkMonitor<T> * net, CNetworkConnection * conn, int nbr, TCPsocket sock) : CNetworkThread<T>(net,conn)
    {
        m_maxClients = nbr;
        m_socket = sock;
        m_recThreads = std::list<CReceiveThread<T>*>();
    }

    /**
     * @brief Destruktor.
     */
    ~CAcceptThread()
    {
        // nic
    }

    /**
     * @brief Zastavuje vlakno, ktere ceka na prichozi spojeni od klientu.
     */
    void stop()
    {
        SDL_LockMutex(this->m_stopMutex);
        if (this->m_pThread == NULL)
        {
            this->er.report(LOG_WARNING, _("%s: AcceptThread allready stopped\n"), AT );
            SDL_UnlockMutex(this->m_stopMutex);
            return;
        }

        if (this->m_running)
        {
            this->m_running = false;
            SDL_KillThread(this->m_pThread);
            this->er.report(LOG_WARNING, _("%s: AcceptThread killed\n"), AT );
        }

        this->m_pThread = NULL;

        if (m_socket != NULL)
        {
            SDLNet_TCP_Close(m_socket);
            m_socket = NULL;
            this->er.report(LOG_INFO, _("%s: Accept socket closed\n"), AT );
        }

        while(!m_recThreads.empty())
        {
            CReceiveThread<T>* rec = this->m_recThreads.front();
            m_recThreads.pop_front();
            this->er.report(LOG_INFO, _("%s: m_recThreads.size: %d (after pop)\n"), AT, m_recThreads.size() );
            rec->stop();
            delete rec;
        }

        this->er.report(LOG_INFO, _("%s: AcceptThread closed\n"), AT );
        SDL_UnlockMutex(this->m_stopMutex);
    }

    /**
     * @brief Funkce, kde probiha cekani na prichozi spojeni. Kdyz prijde, vytvori se cteci ci zapisova vlakna a pridaji se do listu klientu. Pak se opet ceka. Klientum bude navazovano spojeni v intervalu minimalne 10 ms po sobe.
     */
    void run()
    {
        TCPsocket client;
        this->er.report(LOG_INFO, _("%s: Waiting for connection ...\n"), AT );

        while(this->m_running)
        {
            client = SDLNet_TCP_Accept(m_socket);
            if (client)
            {
                this->er.report(LOG_INFO, _("%s: A client connected\n"), AT );

                int sockNbr = this->m_pConnection->addSocket(client);
                CReceiveThread<T>* rec = new CReceiveThread<T>(this->m_pMonitor, this->m_pConnection,sockNbr);

                rec->start();
                m_recThreads.push_front(rec);
                this->er.report(LOG_INFO, _("%s: m_recThreads.size: %d (after push)\n"), AT, m_recThreads.size() );

                client = NULL;
            }
            else
            {
                // wait 10 ms and retry
                SDL_Delay(10);
            }

            for( typename std::list< CReceiveThread<T> *>::iterator itr = m_recThreads.begin(); itr != m_recThreads.end(); itr++)
            {
                CReceiveThread<T>* rec = (CReceiveThread<T> *) *itr;
                if (rec->isRunning())
                {
                    m_recThreads.erase(itr);
                    this->er.report(LOG_INFO, _("%s: m_recThreads.size: %d (after erase)\n"), AT, m_recThreads.size() );
                    this->er.report(LOG_INFO, _("%s: Receiver erased\n"), AT );
                    break;
                }
            }
        }
        stop();
    }

protected:
    int m_maxClients;
    TCPsocket m_socket;

    std::list< CReceiveThread<T> *> m_recThreads;
};





#endif // NETWORKTHREAD_H
