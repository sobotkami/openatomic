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

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <network/network.h>
#include <entities/network/networkMessage.h>
#include <network/client.h>

/**
 * @brief Trida sitoveho klienta. Pracuje se zpravami definovanymi generickym parametrem msg_t.
 */
class CNetworkClient : public IClient, public CNetwork<msg_t>
{
public:
    /**
     * @brief Protoze tato trida je klient, vola kontruktor tridy CNetwork s parametrem FALSE (= klient) a predava ji hostname.
     */
    CNetworkClient(const char * hostname) : CNetwork(false, hostname)
    {
        cout << "CNetworkClient(" << hostname << ")" << endl;
    }

    /**
     * @brief Implementace vsech metod rozhrani IMessages.
     */
    virtual void player(Uint8 player, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, playerstate_t state, playerlife_t life);
    virtual void bomb(Uint8 player, Uint8 x, Uint8 y, Uint16 index, bombstate_t state);
    virtual void inventory(Uint8 player, powerup_t powerup, Sint8 value);
    virtual void updateField(Uint8 x, Uint8 y, Sint8 occupancy, Sint8 powerup);
    virtual void setDefPlPos(Uint8 player, Uint8 x, Uint8 y);
    virtual void changeMenu(enum guiScreen menu);
    virtual void startGame();
    virtual void showResults();
    virtual void exitGame();
    virtual void registerPlayer(Uint8 player, const char *node, plmantype_t control);
    virtual void unregisterPlayer(Uint8 player);

    /**
     * @brief Implementace vsech metod rozhrani IClient.
     */
    virtual void think();
    virtual bool isConnected();

private:
    /**
     * @brief Metody parsujici prichozi zpravy a volaji odpovidajici funkce.
     */
    void parseTypeBomb(msg_t &msg);
    void parseTypePlayer(msg_t &msg);
    void parseTypeInventory(msg_t &msg);
    void parseTypeGameState(msg_t &msg);
    void parseTypeText(msg_t &msg);
    void parseTypeHello(msg_t &msg);
    void parseTypePlMan(msg_t &msg);
    void parseTypeField(msg_t &msg);
    void parseTypePlDefPos(msg_t &msg);

};

#endif // NETWORKCLIENT_H
