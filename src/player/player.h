// Describe the use of the player class.

#ifndef COMP_345_PROJECT_TEAM_N12_PLAYER_H
#define COMP_345_PROJECT_TEAM_N12_PLAYER_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "cards/cards.h"
#include "orders/orders.h"
#include "map/map.h"


using namespace std;

//Forward declaration
class Hand;
class Territory;
class OrdersList;
class Order;

// This class implements the player of the game (who will carry certain information about the territories,
// the hand with cards and the list of orders).
class Player {

private:

    //A collection of territories the Player owns.
    std::vector<Territory*>* territories;

    //Hand object (collection of Card objects) the Player has.
    Hand* hand;

    //An OrdersList object containing Order objects the player has issued.
    OrdersList* ordersList;

    //A collection of players for which this Player cannot attack for the remainder of the turn
    vector<Player*> friendPlayers;

    //True if the player conquered a territory during their turn. Otherwise, false.
    bool conqueredTerritoryInTurn;

public:
    // Default constructor which initializes all the maps and the current game state.
    Player();

    // A copy constructor.
    Player(const Player& pl);

    // Defining the assignment operator.
    Player& operator=(Player pl);

    // Destructor.
    ~Player();

    //Defining the output operator for the player
    friend ostream& operator<<(ostream& out, const Player& pl);

    //A function that will return the territories to be defended.
    std::vector<Territory*> toDefend();

    //A function that will return the territories to be attacked.
    std::vector<Territory*> toAttack();

    //A function that will create an Order object and add it to the list of Orders.
    void issueOrder(Order* order);

    // Getter for the territories.
    [[nodiscard]] std::vector<Territory*>* getTerritories() const;

    // Setter for the territories.
    void setTerritories(const std::vector<Territory*> &territories);

    // Removes a territory from the Player's collection of territories
    void removeTerritory(const Territory& territory);

    // Getter for the hand.
    [[nodiscard]] Hand* getHand() const;

    // Setter for the hand.
    void setHand(const Hand &hand);

    // Getter for the orders list.
    [[nodiscard]] OrdersList* getOrders() const;

    // Setter for the orders list.
    void setOrders(const OrdersList &ordersList);

    // A function used in the assignment operator definition which swaps the member data
    // between two Player objects.
    void swap(Player &first, Player &second);

    //Adds a friend player that cannot be attacked.
    void addFriendPlayer(Player* player);

    //Check if player is a friend
    bool isPlayerFriend(Player* player);

    //Removes all players from the friends player vector
    void clearPlayerFriends();

    //Check if the Player has conquered a territory during their turn.
    bool hasConqueredTerritoryInTurn() const;

    //Setter for the conqueredTerritoryInTurn boolean
    void setConqueredTerritoryInTurn(const bool conqueredTerritoryInTurn);
};

// Free function in order to test the functionality of the Player for assignment #1.
void player_driver(const string &filename);

#endif //COMP_345_PROJECT_TEAM_N12_PLAYER_H
