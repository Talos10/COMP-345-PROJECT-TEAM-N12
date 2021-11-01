#include "orders.h"
#include <stdlib.h>

////////////////////////////Order CLASS////////////////////////////////////
//Default constructor
Order::Order() {}

//Parameterized constructor which initializes an Order with the provided description and effect
Order::Order(const string& description, const string& effect) {
    this->description = new string(description);
    this->effect = new string(effect);
}

//Parameterized constructor which initializes an Order with the provided description, effect and issuingPlayer
Order::Order(const string& description, const string& effect, Player& issuingPlayer) {
    this->description = new string(description);
    this->effect = new string(effect);
    this->issuingPlayer = &issuingPlayer;    //shallow copy because want to refer to same player???
}

//Copy constructor
Order::Order(const Order& order) {
    this->description = new string(*(order.description));
    this->effect = new string(*(order.effect));
    this->issuingPlayer = order.issuingPlayer;    //shallow copy because want to refer to same player???
}

//Destructor
Order::~Order() {
    delete description;
    delete effect;
    //delete issuingPlayer; //should I call this or let the Player destructor handle it?
}

//Getter to retrieve the description of an Order
string* Order::getDescription() const {
    return this->description;
}

//Getter to retrieve the effect of an Order
string* Order::getEffect() const {
    return this->effect;
}

//Getter for the issuing player of the order
Player* Order::getIssuingPlayer() const {
    return this->issuingPlayer;
}

//Setter to set the description of an Order
void Order::setDescription(const string& description) {
    delete this->description;
    this->description = new string(description);
}

//Setter to set the effect of an Order
void Order::setEffect(const string& effect) {
    delete this->effect;
    this->effect = new string(effect);
}

void Order::setIssuingPlayer(Player& issuingPlayer) {
    this->issuingPlayer = &issuingPlayer;
}

//Defining the assignment operator
Order& Order::operator=(const Order& order) {
    if (this != &order) {
        delete this->description;
        delete this->effect;
        this->description = new string(*(order.description));
        this->effect = new string(*(order.effect));
        this->issuingPlayer = order.getIssuingPlayer(); //shallow copy but I think its correct because you dont want to just copy a PLayer (the players should remain the same)??????
    }
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Order& order) {
	out << *(order.description);
	return out;
}

//Checks if an order is valid
bool Order::validate() {
    return true;
}

//Executes an order if it is valid.
void Order::execute() {
    cout << "executing" << endl;
}

////////////////////////////Deploy CLASS////////////////////////////////////
//Default constructor
Deploy::Deploy(): Order("A deploy order tells a certain number of armies taken from the reinforcement pool to deploy to a \n"
                        "target territory owned by the player issuing this order.", "Armies moved from reinforcement pool to target territory.") {}

//Parameterized Constructor
Deploy::Deploy(Player& issuingPlayer, Territory& targetTerritory, int numArmies): Deploy() {
    setIssuingPlayer(issuingPlayer);
    this->targetTerritory = &targetTerritory;
    this->numArmies = numArmies;
}

//Copy constructor
Deploy::Deploy(const Deploy& deploy_order): Order(deploy_order) {}

//Destructor
Deploy::~Deploy() {}

//Checks if a Deploy order is valid
bool Deploy::validate() {
    cout << "Validating Deploy Order..." << endl;
    if (this->targetTerritory->getOwner() != this->getIssuingPlayer()) {
        cout << "Invalid: target territory does not belong to player issuing the order!" << endl;
        return false;
    }
    return true;
}

//Executes a Deploy order
void Deploy::execute() {
    if (this->validate()) {
        cout << "Executing Deploy Order..." << endl;
        this->targetTerritory->addArmies(this->numArmies);
        cout << *this->getEffect() << endl;
    }

}

//Defining the assignment operator
Deploy& Deploy::operator=(const Deploy& deploy_order) {
    Order::operator=(deploy_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Deploy& deploy) {
	out << *deploy.getDescription();
	return out;
}

////////////////////////////Advance CLASS////////////////////////////////////
//Default constructor
Advance::Advance(): Order("An advance order tells a certain number of army units to move from a source territory to a target adjacent territory.", "Moved a number of armies from source territory to adjacent target territory.") {}

//Parameterized Constructor
Advance::Advance(Player& issuingPlayer, Territory& sourceTerritory, Territory& targetTerritory, int numArmies): Advance() {
    setIssuingPlayer(issuingPlayer);
    this->sourceTerritory = &sourceTerritory;
    this->targetTerritory = &targetTerritory;
    this->numArmies = numArmies;
}

//Copy constructor
Advance::Advance(const Advance& adv_order): Order(adv_order) {}

//Desctructor
Advance::~Advance() {};

//Checks if an Advance order is valid
bool Advance::validate() {
    cout << "Validating Advance Order..." << endl;
    if (this->sourceTerritory->getOwner() != this->getIssuingPlayer()) {
        cout << "INVALID: source territory does not belong to player issuing the order!" << endl;
        return false;
    }
    else if (find(sourceTerritory->getNeighbours().begin(), sourceTerritory->getNeighbours().end(), targetTerritory) != sourceTerritory->getNeighbours().end()) {
        cout << "INVALID: target territory is not adjacent to the source territory!" << endl;
        return false;
    }
    return true;
}

//Executes an Advance order
void Advance::execute() {
    if (this->validate()) {
        cout << "Executing Advance Order..." << endl;
        bool bothTerritoriesBelongToTheIssuingPlayer = this->sourceTerritory->getOwner() == this->getIssuingPlayer() && this->targetTerritory->getOwner() == this->getIssuingPlayer();
        if (bothTerritoriesBelongToTheIssuingPlayer) {
            this->targetTerritory->addArmies(this->numArmies);
            this->sourceTerritory->removeArmies(this->numArmies);
            this->setEffect("army units are moved from the source to the target territory.");
        }
        else {
            if (this->getIssuingPlayer()->isPlayerFriend(this->sourceTerritory->getOwner())) {
                cout << "You cannot attack this player!" << endl;
                return;
            }
            int attackingArmies = this->numArmies;
            int defendingArmies = targetTerritory->getNumberOfArmies();
            this->sourceTerritory->removeArmies(attackingArmies);

            for (int i = 0; i < attackingArmies; i++) {
                int chanceOfAttack = rand() % 100 + 1;
                if (chanceOfAttack <= 60) {
                    if (defendingArmies == 0) {
                        break;
                    }
                    defendingArmies--;
                }
            }

            for (int i = 0; i < defendingArmies; i++) {
                int chanceOfDefence = rand() % 100 + 1;
                if (chanceOfDefence <= 70) {
                    attackingArmies--;
                }
            }

            //All enemies dead and you still have attacking armies
            if (attackingArmies > 0 && !defendingArmies) {
                targetTerritory->setOwner(this->getIssuingPlayer());
                targetTerritory->setNumberOfArmies(attackingArmies);
                //seems like I need a DECK to draw??? Maybe we should make the deck a global variable???
//                this->getIssuingPlayer().draw();
//                deck.draw(this->getIssuingPlayer()->getHand());
                this->setEffect("Target territory successfully captured!");
            }
            else {
                targetTerritory->setNumberOfArmies(defendingArmies);
                this->setEffect("The defending armies won the battle. You did not conquer the target territory!");
            }
            cout << *this->getEffect() << endl;
        }
    }
}

//Defining the assignment operator
Advance& Advance::operator=(const Advance& adv_order) {
    Order::operator=(adv_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Advance& advance) {
	out << *advance.getDescription();
	return out;
}

////////////////////////////Bomb CLASS////////////////////////////////////
//Default constructor
Bomb::Bomb(): Order("A bomb order targets a territory owned by another player than the one issuing the order. Its result is to remove half of the armies from this territory.", "Half of the armies removed from target territory.") {}

//Parameterized Constructor
Bomb::Bomb(Player& issuingPlayer, Territory& targetTerritory): Bomb() {
    setIssuingPlayer(issuingPlayer);
    this->targetTerritory = &targetTerritory;
}

//Copy constructor
Bomb::Bomb(const Bomb& bomb_order): Order(bomb_order) {}

//Destructor
Bomb::~Bomb() {};

//Checks if a Bomb order is valid
bool Bomb::validate() {
    cout << "Validating Bomb Order..." << endl;
    if (this->targetTerritory->getOwner() == this->getIssuingPlayer()) {
        cout << "INVALID: target territory belongs to the player that issued the order!" << endl;
    }
    else if (this->getIssuingPlayer()->isPlayerFriend(this->targetTerritory->getOwner())) {
        cout << "INVALID: You cannot attack this player!" << endl;
    }
    else {
        list<Territory*> neighbourTerritories = this->targetTerritory->getNeighbours();
        for (Territory* neighbour : neighbourTerritories) {
            if (neighbour->getOwner() == this->getIssuingPlayer()) {
                return true;
            }
        }
    }
    return false;
}

//Executes a Bomb order
void Bomb::execute() {
    if (this->validate()) {
        cout << "Executing Bomb Order..." << endl;
        //SHOULD I ROUND DOWN?????????
        this->targetTerritory->removeArmies(0.5*this->targetTerritory->getNumberOfArmies());
        this->setEffect("removed half of the armies from the target territory!");
        cout << *this->getEffect() << endl;
    }
}

//Defining the assignment operator
Bomb& Bomb::operator=(const Bomb& bomb_order) {
    Order::operator=(bomb_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Bomb& bomb) {
	out << *bomb.getDescription();
	return out;
}

////////////////////////////Blockade CLASS////////////////////////////////////
//Default constructor
Blockade::Blockade(): Order("A blockade order targets a territory that belongs to the player issuing the order", "double  the  number of  armies on the territory  and to  transfer the ownership  of  the  territory to the Neutral player.") {}

//Parameterized Constructor
Blockade::Blockade(Player& issuingPlayer, Territory& targetTerritory): Blockade() {
    setIssuingPlayer(issuingPlayer);
    this->targetTerritory = &targetTerritory;
}

//Copy constructor
Blockade::Blockade(const Blockade& blockade_order): Order(blockade_order) {}

//Destructor
Blockade::~Blockade() {};

//Checks if a Blockade order is valid
bool Blockade::validate() {
    cout << "Validating Blockade Order..." << endl;
    if (this->targetTerritory->getOwner() != this->getIssuingPlayer()) {
        return false;
    }
    return true;
}

//Executes a Blockade order
void Blockade::execute() {
    if (this->validate()) {
        cout << "Executing Blockcade Order..." << endl;
        this->targetTerritory->addArmies(this->targetTerritory->getNumberOfArmies());
        this->getIssuingPlayer()->removeTerritory(*this->targetTerritory);

        //Neutral player owner
        this->targetTerritory->setOwner(nullptr);
        this->getEffect();
    }
}

//Defining the assignment operator
Blockade& Blockade::operator=(const Blockade& blockade_order) {
    Order::operator=(blockade_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Blockade& blockade) {
	out << *blockade.getDescription();
	return out;
}

////////////////////////////Airlift CLASS////////////////////////////////////
//Default constructor
Airlift::Airlift(): Order("An airlift order tells a certain number of armies taken from a source territory to be moved to a target territory, the source and the target territory being owned by the player issuing the order.", "Armies moved from source territory to target territory") {}

//Parameterized Constructor
Airlift::Airlift(Player& issuingPlayer, Territory& sourceTerritory, Territory& targetTerritory, int numArmies): Airlift() {
    setIssuingPlayer(issuingPlayer);
    this->sourceTerritory = &sourceTerritory;
    this->targetTerritory = &targetTerritory;
    this->numArmies = numArmies;
}

//Copy constructor
Airlift::Airlift(const Airlift& airlift_order): Order(airlift_order) {}

//Destructor
Airlift::~Airlift() {};

//Checks if an Airlift order is valid
bool Airlift::validate() {
    cout << "Validating Airlift Order..." << endl;
    if (this->sourceTerritory->getOwner() != this->getIssuingPlayer()) {
        cout << "INVALID: Source territory does not belong to the issuing player!" << endl;
        return false;
    }
    else if (this->targetTerritory->getOwner() != this->getIssuingPlayer()) {
        cout << "INVALID: Target territory does not belong to the issuing player!" << endl;
        return false;
    }
    return true;
}

//Executes an Airlift order
void Airlift::execute() {
    if (this->validate()) {
        cout << "Executing Airlift Order..." << endl;
        this->sourceTerritory->removeArmies(numArmies);
        this->targetTerritory->addArmies(numArmies);
        cout << *this->getEffect() << endl;
    }
}

//Defining the assignment operator
Airlift& Airlift::operator=(const Airlift& airlift_order) {
    Order::operator=(airlift_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Airlift& airlift) {
	out << *airlift.getDescription();
	return out;
}

////////////////////////////Negotiate CLASS////////////////////////////////////
//Default constructor
Negotiate::Negotiate(): Order("A negotiate order targets an enemy player. It results in the target player and the player issuing the order to not be able to successfully attack each others’ territories for the remainder of the turn.", "Players cannot attack eachother for remainder of the turn.") {}

//Parameterized Constructor
Negotiate::Negotiate(Player& issuingPlayer, Player& enemyPlayer): Negotiate() {
    setIssuingPlayer(issuingPlayer);
    this->enemyPlayer = &enemyPlayer;
}

//Copy constructor
Negotiate::Negotiate(const Negotiate& negotiate_order): Order(negotiate_order) {}

//Destructor
Negotiate::~Negotiate() {};

//Checks if a Negotiate order is valid
bool Negotiate::validate() {
    cout << "Validating Negotiate Order..." << endl;
    if (this->enemyPlayer == this->getIssuingPlayer() || this->enemyPlayer == nullptr) {
        cout << "INVALID: Issuing player and enemy player are the same!" << endl;
        return false;
    }
    return true;
}

//Executes a Negotiate order
void Negotiate::execute() {
    if (this->validate()) {
        cout << "Executing Negotiate Order..." << endl;
        this->getIssuingPlayer()->addFriendPlayer(this->enemyPlayer);
        this->enemyPlayer->addFriendPlayer(this->getIssuingPlayer());
        cout << *this->getEffect() << endl;
    }
}

//Defining the assignment operator
Negotiate& Negotiate::operator=(const Negotiate& negotiate_order) {
    Order::operator=(negotiate_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Negotiate& negotiate) {
	out << *negotiate.getDescription();
	return out;
}

////////////////////////////OrdersList CLASS////////////////////////////////////
//Default constructor
OrdersList::OrdersList() {
    this->orders = new vector<Order*>();
}

//Copy constructor
OrdersList::OrdersList(const OrdersList& o_list): OrdersList() {
    for (int i = 0; i < o_list.getOrders()->size(); i++) {
       Order* o = new Order(*(o_list.orders)->at(i));
       this->addOrder(o);
    }
}

//Destructor
OrdersList::~OrdersList() {
    //delete each order on heap
    for (Order* order : *this->orders) {
        delete order;
    }
    this->orders->clear();  //delete all Order pointers in vector
    delete this->orders;    //delete orders pointer
    this->orders = nullptr;
}

//Move an Order in the vector to a new index by providing its current index and the index it should be moved to
void OrdersList::move(int currentIndex, int newIndex) {
    int order_list_size = this->orders->size();
    if (order_list_size == 0) {
        cout << "Cannot move an order because there are no orders in the OrdersList!" << endl;
    }
    else if (currentIndex < order_list_size && newIndex < order_list_size && currentIndex >= 0 && newIndex >= 0) {
        Order* orderToMove = this->orders->at(currentIndex);
        this->orders->insert(this->orders->begin() + newIndex, orderToMove);
        this->orders->erase(this->orders->begin() + currentIndex);
    }
    else {
        cout << "Invalid Index position specified" << endl;
    }
}

//Remove an order in the vector by specifying its respective index
void OrdersList::remove(const int orderIndex) {
    if (this->orders->size() == 0) {
        cout << "Cannot remove an order because there are no orders in the OrdersList!" << endl;
    }
    else if (orderIndex >= 0 && orderIndex < this->orders->size()) {
        delete this->orders->at(orderIndex);    //free memory
        this->orders->erase(this->orders->begin() + orderIndex);    //deletes pointer from vector
    }
    else {
        cout << "Invalid order position" << endl;
    }
}

//Getter for the vector of orders
vector<Order*>* OrdersList::getOrders() const {
    return this->orders;
}

//Add an order to the vector by providing an Order
void OrdersList::addOrder(Order* order) {
    if (order == nullptr) {
        cout << "null pointer! Order cannot be added!" << endl;
    }
    else {
        this->orders->push_back(order);
    }
}

//Defining the assignment operator
OrdersList& OrdersList::operator=(const OrdersList& o_list) {
    if (this != &o_list) {
        //delete all orders in left hand side OrdersList
        for (int index = 0; index < this->orders->size(); index++) {
            this->remove(index);
        }
        //add all orders from right hand side OrdersList into left hand side
        for (int i = 0; i < o_list.getOrders()->size(); i++) {
            Order* o = new Order(*(o_list.orders)->at(i));
            this->addOrder(o);
        }
    }
    return *this;
}

//Defining the addition operator
void OrdersList::operator+(Order* order) {
    this->addOrder(order);
}

//Defining the output operator
ostream& operator<<(ostream& out, const OrdersList& ol) {
	out << "The OrdersList contains " << ol.orders->size() << " orders:" << endl;
    for (Order* order : *(ol.orders)) {
        out << *order << endl;
    }
	return out;
}

// Free function in order to test the functionality of the Orders.cpp for assignment #1.
void orders_driver() {
    cout << "\n##############################################################" << endl;
    cout << "################### Running Orders driver! ###################" << endl;
    cout << "##############################################################" << endl;
    OrdersList* orders_list = new OrdersList();
    Deploy* deploy = new Deploy();
    Advance* advance = new Advance();
    Bomb* bomb = new Bomb();
    Blockade* blockade = new Blockade();
    Airlift* airlift = new Airlift();
    Negotiate* negotiate = new Negotiate();
    Bomb* copyBomb = new Bomb(*bomb);

    //add orders
    cout << "Adding orders to the OrdersList..." << endl;
    orders_list->addOrder(deploy);
    orders_list->addOrder(advance);
    orders_list->addOrder(bomb);
    *orders_list + blockade;
    *orders_list + airlift;
    *orders_list + negotiate;
    orders_list->addOrder(copyBomb);
    cout << orders_list << endl;

    //move orders
    orders_list->move(1, 4);
    cout << "#####Moved order at index 1 to position 4!#####" << endl;
    cout << orders_list << endl;

    //remove orders
    orders_list->remove(3);
    cout << "#####Removed order at index 3!#####" << endl;
    cout << orders_list << endl;

    //validate orders
    cout << "#####Checking if order at index 1 is valid#####" << endl;
    bool isOrderValid = orders_list->getOrders()->at(1)->validate();
    if (isOrderValid) {
        cout << "The order is valid!" << endl << endl;
    }

    //execute orders
    cout << "#####Executing order at index 2#####" << endl;
    orders_list->getOrders()->at(2)->execute();

    //Delete the orders list from the heap
    delete orders_list;
}

