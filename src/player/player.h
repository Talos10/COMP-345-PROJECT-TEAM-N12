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


};

// Free function in order to test the functionality of the Player for assignment #1.
void player_driver(const string &filename);

#endif //COMP_345_PROJECT_TEAM_N12_PLAYER_H
