#include "Orders.h"

////////////////////////////Order CLASS////////////////////////////////////
//Default constructor
Order::Order() : Order("generic order", "no effect") {}

//Parameterized constructor which initializes an Order with the provided description and effect
Order::Order(string description, string effect) {
    this->description = new string(description);
    this->effect = new string(effect);
}

//Copy constructor
Order::Order(const Order& order) {
    this->description = new string(*(order.description));
    this->effect = new string(*(order.effect));
}

//Destructor
Order::~Order() {
    delete description;
    delete effect;
}

//Getter to retrieve the description of an Order
string Order::getDescription() const {
    return *(this->description);
}

//Getter to retrieve the effect of an Order
string Order::getEffect() const {
    return *(this->effect);
}

//Setter to set the description of an Order
void Order::setDescription(const string& description) {
    *(this->description) = description;
}

//Setter to set the effect of an Order
void Order::setEffect(const string& effect) {
    *(this->effect) = effect;
}

//Defining the equality operator
Order& Order::operator=(const Order& order) {
    this->description = new string(*(order.description));
    this->effect = new string(*(order.effect));
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Order& order) {
	out << *(order.description);
	return out;
}

////Checks if an order is valid
bool Order::validate() {
    return true;
}

//Executes an order if it is valid.
void Order::execute() {
    cout << "executing" << endl;
}

////////////////////////////Deploy CLASS////////////////////////////////////
//Default constructor
Deploy::Deploy(): Order("Deploy Default", "Deploy effect") {}

//Copy constructor
Deploy::Deploy(const Deploy& deploy_order): Order(deploy_order) {}

//Destructor
Deploy::~Deploy() {};

//Checks if a Deploy order is valid
bool Deploy::validate() {
    cout << "Validating Deploy Order" << endl;
    return true;
}

//Executes a Deploy order
void Deploy::execute() {
    if (this->validate()) {
        cout << "Executing Deploy Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

//Defining the equality operator
Deploy& Deploy::operator=(const Deploy& deploy_order) {
    Order::operator=(deploy_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Deploy& deploy) {
	out << deploy.getDescription();
	return out;
}

////////////////////////////Advance CLASS////////////////////////////////////
//Default constructor
Advance::Advance(): Order("Advance Default", "Advance effect") {}

//Copy constructor
Advance::Advance(const Advance& adv_order): Order(adv_order) {}

//Desctructor
Advance::~Advance() {};

//Checks if an Advance order is valid
bool Advance::validate() {
    cout << "Validating Advance Order" << endl;
    return true;
}

//Executes an Advance order
void Advance::execute() {
    if (this->validate()) {
        cout << "Executing Advance Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

//Defining the equality operator
Advance& Advance::operator=(const Advance& adv_order) {
    Order::operator=(adv_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Advance& advance) {
	out << advance.getDescription();
	return out;
}

////////////////////////////Bomb CLASS////////////////////////////////////
//Default constructor
Bomb::Bomb(): Order("Bomb Default", "Bomb effect") {}

//Copy constructor
Bomb::Bomb(const Bomb& bomb_order): Order(bomb_order) {}

//Destructor
Bomb::~Bomb() {};

//Checks if a Bomb order is valid
bool Bomb::validate() {
    cout << "Validating Bomb Order" << endl;
    return true;
}

//Executes a Bomb order
void Bomb::execute() {
    if (this->validate()) {
        cout << "Executing Bomb Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

//Defining the equality operator
Bomb& Bomb::operator=(const Bomb& bomb_order) {
    Order::operator=(bomb_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Bomb& bomb) {
	out << bomb.getDescription();
	return out;
}

////////////////////////////Blockade CLASS////////////////////////////////////
//Default constructor
Blockade::Blockade(): Order("Blockade Default", "Blockade effect") {}

//Copy constructor
Blockade::Blockade(const Blockade& blockade_order): Order(blockade_order) {}

//Destructor
Blockade::~Blockade() {};

//Checks if a Blockade order is valid
bool Blockade::validate() {
    cout << "Validating Blockade Order" << endl;
    return true;
}

//Executes a Blockade order
void Blockade::execute() {
    if (this->validate()) {
        cout << "Executing Blockcade Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
    
}

//Defining the equality operator
Blockade& Blockade::operator=(const Blockade& blockade_order) {
    Order::operator=(blockade_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Blockade& blockade) {
	out << blockade.getDescription();
	return out;
}

////////////////////////////Airlift CLASS////////////////////////////////////
//Default constructor
Airlift::Airlift(): Order("Airlift Default", "Airlift effect") {}

//Copy constructor
Airlift::Airlift(const Airlift& airlift_order): Order(airlift_order) {}

//Destructor
Airlift::~Airlift() {};

//Checks if an Airlift order is valid
bool Airlift::validate() {
    cout << "Validating Airlift Order" << endl;
    return true;
}

//Executes an Airlift order
void Airlift::execute() {
    if (this->validate()) {
        cout << "Executing Airlift Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

//Defining the equality operator
Airlift& Airlift::operator=(const Airlift& airlift_order) {
    Order::operator=(airlift_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Airlift& airlift) {
	out << airlift.getDescription();
	return out;
}

////////////////////////////Negotiate CLASS////////////////////////////////////
//Default constructor
Negotiate::Negotiate(): Order("Negotiate Default", "Negotiate effect") {}

//Copy constructor
Negotiate::Negotiate(const Negotiate& negotiate_order): Order(negotiate_order) {}

//Destructor
Negotiate::~Negotiate() {};

//Checks if a Negotiate order is valid
bool Negotiate::validate() {
    cout << "Validating Negotiate Order" << endl;
    return true;
}

//Executes a Negotiate order
void Negotiate::execute() {
    cout << "Executing Negotiate Order" << endl;
}

//Defining the equality operator
Negotiate& Negotiate::operator=(const Negotiate& negotiate_order) {
    Order::operator=(negotiate_order);
    return *this;
}

//Defining the output operator
ostream& operator<<(ostream& out, const Negotiate& negotiate) {
	out << negotiate.getDescription();
	return out;
}

////////////////////////////OrdersList CLASS////////////////////////////////////
//Default constructor
OrdersList::OrdersList() {
    this->orders = new vector<Order*>();
}

//Parameterized constructor which instantiates the vector of orders with the provided vector of orders
OrdersList::OrdersList(vector<Order*>& orders) {
    this->orders = new vector<Order*>(orders);
}

//Copy constructor
OrdersList::OrdersList(const OrdersList& o_list) {
    //IS THIS CORRECT???
    this->orders = new vector<Order*>(*o_list.orders);
}

//Destructor
OrdersList::~OrdersList() {
    delete orders;  //might be wrong???
    //HOW ABOUT THIS???
    for (Order* order : *this->orders) {
        delete order;
    }
    this->orders->clear();
    delete this->orders;
    this->orders = nullptr;
} 

//Move an Order in the vector to a new index by providing its current index and the index it should be moved to
void OrdersList::move(int currentIndex, int newIndex) {
    int order_list_size = this->orders->size();
    if (currentIndex < order_list_size && newIndex < order_list_size && currentIndex >= 0 && newIndex >= 0) {
        Order* orderToMove = this->orders->at(currentIndex);
        this->orders->erase(this->orders->begin() + currentIndex);
        this->orders->insert(this->orders->begin() + newIndex, orderToMove);
    }
    else {
        cout << "Invalid Index position specified" << endl;
    }
}

//Remove an order in the vector by specifying its respective index
void OrdersList::remove(const int orderIndex) {
    if (orderIndex >= 0 && orderIndex < this->orders->size()) {
        this->orders->erase(this->orders->begin() + orderIndex);
    }
    else {
        cout << "Invalid order position" << endl;
    }
}

//Getter for the vector of orders
vector<Order*>& OrdersList::getOrders() const {
    return *(this->orders);
}

//Add an order to the vector by providing an Order
void OrdersList::addOrder(Order* order) {
    this->orders->push_back(order);
}

//Add an order to the vector at a specific index by providing an Order and the desired index position
void OrdersList::addOrder(Order* order, int index) {
        this->orders->insert(this->orders->begin() + index, order);
}

//Defining the equality operator
OrdersList& OrdersList::operator=(const OrdersList& ordersList) {
    this->orders = new vector<Order*>(*(ordersList.orders));
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
    cout << "\n### Running Orders driver! ###" << endl;
    OrdersList orders_list;
    Order* order1 = new Order();
    Deploy* deploy = new Deploy();
    Advance* advance = new Advance();
    Bomb* bomb = new Bomb();
    Blockade* blockade = new Blockade();
    Airlift* airlift = new Airlift();
    Negotiate* negotiate = new Negotiate();
    Bomb copyBomb(*bomb);

    //add orders
    orders_list.addOrder(order1);
    orders_list.addOrder(deploy);
    orders_list.addOrder(advance);
    orders_list.addOrder(bomb);
    orders_list + blockade;
    orders_list + airlift;
    orders_list + negotiate;
    orders_list.addOrder(&copyBomb);
    cout << orders_list << endl;
    
    //retrieve orders
    vector<Order*> orders = orders_list.getOrders();

    //move orders
    orders_list.move(1, 4);
    cout << "Moved order at index 1 to index 4!" << endl; 
    cout << orders_list << endl;

    //remove orders
    orders_list.remove(3);
    cout << "Removed order at index 3!" << endl; 
    cout << orders_list << endl;

    //validate orders
    cout << "Checking if order 1 is valid: " << endl;
    if (orders[1]->validate()) {
        cout << "The order is valid!" << endl;
    }

    //execute orders
    orders_list.getOrders()[1]->execute();

    cout << orders_list << endl;
}
