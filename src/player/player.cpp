#include "player.h"
#include <iostream>
#include <vector>
#include <functional>
#include "cards/cards.h"
#include "orders/Orders.h"

using namespace std;

// The implementation file of the Player class.

// Default constructor which initializes the territories, hand and ordersList for the Player.
Player::Player(){

    //Territories, hand and ordersList initialization.
    territories = new std::vector<pair <string, string>>{};
    hand = new Hand();
    ordersList = new OrdersList();
};

// Copy constructor.
Player::Player(const Player &pl) {
    this->territories = new std::vector<pair <string, string>>(*pl.territories);
    this->hand = new Hand(*pl.hand);
    this->ordersList = new OrdersList(*pl.ordersList);
}

// Swaps the member data between two Player objects.
void Player::swap(Player &first, Player &second) {
    std::swap(first.territories, second.territories);
    std::swap(first.hand, second.hand);
    std::swap(first.ordersList, second.ordersList);
}

// Destructor.
Player::~Player() {
    delete territories;
    delete hand;
    delete ordersList;
}

ostream& operator<<(ostream& os, const Player& pl)
{
    os << "\nTerritories: \n" << pl.getTerritories() << "\nHand: \n" << *pl.getHand() << "\nOrdersList: \n" << *pl.getOrders();
    return os;
}

//Defining the equality operator
Player& Player::operator=(const Player& pl) {
    this->territories = new std::vector<pair <string, string>>(*pl.territories);
    this->hand = new Hand(*pl.hand);
    this->ordersList = new OrdersList(*pl.ordersList);
    return *this;
}

// The way this method works is that it first creates a local temporary copy of the given object (called pl)
// and method calls the swap function on the caller object (which is a Player obj that was created
// with the default constructor) and on the locally created object. The swap method will effectively swap
// the member data between those two objects and so the caller object will now have the properties of the
// given object.
Player &Player::operator=(Player pl) {
    swap(*this, pl);
    return *this;
}

// Getter for the territories.
std::vector<pair <string, string>> *Player::getTerritories() const {
    return territories;
}

// Setter for the territories.
void Player::setTerritories(const std::vector<pair <string, string>> &territories) {
    this->territories = new std::vector(territories);
}

//Defining the output operator
ostream& operator<<(ostream& out, const std::vector<pair <string, string>>& territories) {
    out << "The territories list contains " << territories.size() << " territories:" << endl;
    for (pair <string, string> territory : territories) {
        out << territory.first << endl;
    }
    return out;
}

// Getter for the hand.
Hand* Player::getHand() const {
    return hand;
}

// Setter for the hand.
void Player::setHand(const Hand &hand) {
    this->hand = new Hand(hand);
}

// Getter for the orders list.
OrdersList* Player:: getOrders() const {
    return ordersList;
}

// Setter for the orders list.
void Player::setOrders(const OrdersList &ordersList) {
    this->ordersList = new OrdersList(ordersList);
}

//A function which will go through the collection of territories the player owns and
//check if that territory has the attribute "defend". If it does, it is added to a temporary
//list which is then returned. This list will contain all the territories to be defended.
vector<pair<string, string>> Player:: toDefend(){

    std::vector<pair <string, string>> territories2Defend;

    for(std::pair <string, string> territory: *territories){
        if(territory.second == "defend"){
            territories2Defend.push_back(territory);
        }
    }

    return territories2Defend;

}

//A function which will go through the collection of territories the player owns and
//check if that territory has the attribute "attack". If it does, it is added to a temporary
//list which is then returned. This list will contain all the territories to be attacked.
vector<pair<string, string>> Player::toAttack() {

    std::vector<pair <string, string>> territories2Attack;

    for(std::pair <string, string> territory: *territories){
        if(territory.second == "attack"){
            territories2Attack.push_back(territory);
        }
    }

    return territories2Attack;
}

//A function which creates an Order object and adds it to the list of Orders.
void Player::issueOrder(const string &description, const string &effect){
    ordersList->addOrder(*(new Order(description, effect)));
}

// Free function in order to test the functionality of the Player for assignment #1.
void player_driver() {

    cout << "\nPlayer driver function" << endl;

    //Create player1 object
    Player player1;

    //Set the territories with dummy data
    player1.setTerritories({{"t1", "attack"}, {"t2", "defend"}, {"t3", "defend"}, {"t4", "attack"}});

    //Output territories to attack
    cout << "\nTerritories to attack:" << endl;
    for(pair<string, string> territory: player1.toAttack()){
        cout << "\n" << territory.first << endl;
    }

    //Output territories to defend
    cout << "\nTerritories to defend:" << endl;
    for(pair<string, string> territory: player1.toDefend()){
        cout << "\n" << territory.first << endl;
    }

    //Issue an order
    player1.issueOrder("test description #1", "test effect #1");

    //Output the list of orders
    cout << "Issued orders " << *player1.getOrders() << endl;

    //Issue an additional order
    player1.issueOrder("test description #2", "test effect #2");

    //Output the list of orders with the 2 orders
    cout << "Issued orders " << *player1.getOrders() << endl;

    //Testing copy constructor
    Player player2 = {player1};


    cout << "player1 object: \n" << player1 << endl;

    cout << "player2 object: \n" << player2 << endl;

    cout << "player1 address: " << &player1 << "\tplayer2 address " << &player2 << endl;
}