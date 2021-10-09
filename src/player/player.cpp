#include "player.h"



using namespace std;

// The implementation file of the Player class.

// Default constructor which initializes the territories, hand and ordersList for the Player.
Player::Player(){

    //Territories, hand and ordersList initialization.
    territories = new std::vector<Territory*>{};
    hand = new Hand();
    ordersList = new OrdersList();
};

// Copy constructor.
Player::Player(const Player &pl) {
    this->territories = new std::vector<Territory*>(*pl.territories);
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

//Implementing the output operator
ostream& operator<<(ostream& out, const Player &pl)
{

    out << "The territories collection contains " << pl.territories->size() << " territories:" << endl;
    for (Territory* territory : *pl.territories) {
        out << territory << endl;
    }

    out << "\nHand: \n" << *pl.getHand() << "\nOrdersList: \n" << *pl.getOrders();
    return out;
}


// The way this method works is that it first creates a local temporary copy of the given object (called pl)
// and method calls the swap function on the caller object (which is a Player obj that was created
// with the default constructor) and on the locally created object. The swap method will effectively swap
// the member data between those two objects and so the caller object will now have the properties of the
// given object. [Assignment operator]
Player &Player::operator=(Player pl) {
    swap(*this, pl);
    return *this;
}

// Getter for the territories.
std::vector<Territory*>* Player::getTerritories() const {
    return territories;
}

// Setter for the territories.
void Player::setTerritories(const std::vector<Territory*> &territories) {
    delete this->territories;
    this->territories = new std::vector(territories);
}

//Defining the output operator
ostream& operator<<(ostream& out, const std::vector<Territory*> &territories) {
    out << "The territories collection contains " << territories.size() << " territories:" << endl;
    for (Territory* territory : territories) {
        out << territory << endl;
    }
    return out;
}

// Getter for the hand.
Hand* Player::getHand() const {
    return hand;
}

// Setter for the hand.
void Player::setHand(const Hand &hand) {
    delete this->hand;
    this->hand = new Hand(hand);
}

// Getter for the orders list.
OrdersList* Player:: getOrders() const {
    return ordersList;
}

// Setter for the orders list.
void Player::setOrders(const OrdersList &ordersList) {
    delete this->ordersList;
    this->ordersList = new OrdersList(ordersList);
}

//A function which will go through the collection of territories the player owns and
//check if that territory has the attribute "defend". If it does, it is added to a temporary
//list which is then returned. This list will contain all the territories to be defended.
vector<Territory*> Player:: toAttack(){

    std::vector<Territory*> territories2Attack;
    int i = 0;

    //getting an arbitrary set of territories to defend
    for(auto i = 0; i < territories->size()/2; i++){
        territories2Attack.push_back(territories->at(i));
    }

    return territories2Attack;

}

//A function which will go through the collection of territories the player owns and
//check if that territory has the attribute "attack". If it does, it is added to a temporary
//list which is then returned. This list will contain all the territories to be attacked.
vector<Territory*> Player::toDefend() {

    std::vector<Territory*> territories2Defend;

    //getting an arbitrary set of territories to attack
    for(auto i = territories->size()/2; i < territories->size(); i++){
        territories2Defend.push_back(territories->at(i));
    }

    return territories2Defend;
}

//A function which creates an Order object and adds it to the list of Orders.
void Player::issueOrder(Order* order){
    ordersList->addOrder(order);
}

// Free function in order to test the functionality of the Player for assignment #1.
void player_driver() {

    //Load the map
    string filename = "canada-map.txt";
    Map* map = MapLoader::load(filename);

    cout << "\n***************************Player driver function***************************" << endl;

    //Create player1 object
    Player* player1 = new Player();

    //Set the territories with sample data from the Map
    player1->setTerritories({map->getTerritoryByID(1),map->getTerritoryByID(2), map->getTerritoryByID(3), map->getTerritoryByID(4)});

    //Output the list of territories
    cout << "\nList of Territories:" << endl;
    for(auto i = 0; i < player1->getTerritories()->size(); i++){
        cout << "\n" << player1->getTerritories()->at(i);
    }
    cout << endl;

    //Testing the hand (cards) functionality with the player object
    Card *c1 = new Card();
    Card *c2 = new Card();
    Card *c3 = new Card();

    vector<Card*>* cards = new vector<Card*>();
    cards->emplace_back(c1);
    cards->emplace_back(c2);
    cards->emplace_back(c3);
    Hand* hand = new Hand(*cards);
    player1->setHand(*hand);


    //Output territories to attack
    cout << "\nTerritories to attack:" << endl;
    for(auto i = 0; i < player1->toAttack().size(); i++){
        cout << "\n" << player1->toAttack().at(i);
    }
    cout << endl;

    //Output territories to defend
    cout << "\nTerritories to defend:" << endl;
    for(auto i = 0; i < player1->toDefend().size(); i++){
        cout << "\n" << player1->toDefend().at(i);
    }
    cout << "\n" << endl;

    //Issue an order
    player1->issueOrder(new Deploy());

    //Output the list of orders
    cout << "Issued orders: \n" << *player1->getOrders() << endl;

    //Issue an additional order
    player1->issueOrder(new Advance());

    //Output the list of orders with the 2 orders
    cout << "Issued orders after adding a 2nd order: \n" << *player1->getOrders() << endl;

    //Testing copy constructor
    Player* player2 = new Player(*player1);


    //Proof that the copy constructor does a deep copy
    cout << "Printing player1 object... \n" << *player1 << endl;
    cout << "Printing player2 object... \n" << *player2 << endl;

    //Players have different addresses
    cout << "player1 address: " << &player1 << "\tplayer2 address " << &player2 << endl;

    //Delete the objects on the heap
    delete player1;
    delete player2;

}