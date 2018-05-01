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

#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H


#include <SDL/SDL.h>

#include <list>


/**
 * @brief Trida CNetworkMonitor. Spravuje vstupne/vystupni fronty zprav.
 */
template<typename T>
class CNetworkMonitor
{
public:
	/**
	 * @brief Konstruktor. Inicializuje promenne a fronty zprav.
	 */
	CNetworkMonitor()
	{
		m_pInputMutex = SDL_CreateMutex();
		m_pOutputMutex = SDL_CreateMutex();
		m_pOutputCondition = SDL_CreateCond();

		input = std::list<T>();
		output = std::list<T>();
	}

	/**
	 * @brief Destruktor.
	 */
	~CNetworkMonitor()
	{
		SDL_DestroyMutex(m_pInputMutex);
		SDL_DestroyMutex(m_pOutputMutex);
		SDL_DestroyCond(m_pOutputCondition);
	}

	/**
	 * @brief Vloz zpravu do fronty prichozich zprav.
	 * @param b Zprava, ktera bude vlozena.
	 */
	void insertIncoming(T b)
	{
		SDL_LockMutex(m_pInputMutex);
		input.push_back(b);
		SDL_UnlockMutex(m_pInputMutex);
	}

	/**
	 * @brief Vloz zpravu do fronty odchozich zprav.
	 * @param b Zprava, ktera bude vlozena.
	 */
	void insertOutcoming(T b)
	{
		SDL_LockMutex(m_pOutputMutex);
		output.push_back(b);
		SDL_CondBroadcast(m_pOutputCondition);
		SDL_UnlockMutex(m_pOutputMutex);
	}

	/**
	 * @brief Vyzvedni zpravu z prichozi fronty
	 * @param b Zprava, ktera bude naplnena vyzvednutou zpravou z fronty.
	 */
	void getOutcoming(T* b)
	{
		SDL_LockMutex(m_pOutputMutex);
		while(output.empty())
		{
			SDL_CondWait(m_pOutputCondition,m_pOutputMutex);
		}

		*b = output.front();
		output.pop_front();
		SDL_UnlockMutex(m_pOutputMutex);
	}

	/**
	 * @brief Vyzvedni zpravu z prichozi fronty.
	 * @param b Zprava, ktera bude naplnena vyzvednutou zpravou z fronty.
	 * @return Vraci TRUE, pokud byla nejaka zprava nactena.
	 */
	bool getIncoming(T *b)
	{
		bool retVal = false;
		SDL_LockMutex(m_pInputMutex);
		if (!input.empty())
		{
			retVal = true;
			*b = input.front();
			input.pop_front();
		}
		SDL_UnlockMutex(m_pInputMutex);
		return retVal;
	}

protected:
	SDL_mutex* m_pInputMutex;
	SDL_mutex* m_pOutputMutex;
	SDL_cond* m_pOutputCondition;

	std::list<T> input;
	std::list<T> output;
private:
};



#endif // NETWORKMONITOR_H
