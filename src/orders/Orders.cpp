#include "Orders.h"

////////////////////////////Order CLASS////////////////////////////////////
Order::Order() : Order("generic order", "no effect") {}
Order::Order(string description, string effect) {
    this->description = new string(description);
    this->effect = new string(effect);
}
Order::Order(const Order& order) {
    this->description = new string(*(order.description));
    this->effect = new string(*(order.effect));

}
Order::~Order() {
    delete description;
    delete effect;
}

string Order::getDescription() const {
    return *(this->description);
}

string Order::getEffect() const {
    return *(this->effect);
}

void Order::setDescription(const string& description) {
    *(this->description) = description;
}

void Order::setEffect(const string& effect) {
    *(this->effect) = effect;
}

Order& Order::operator=(const Order& order) {
    this->description = new string(*(order.description));
    this->effect = new string(*(order.effect));
    return *this;
}

ostream& operator<<(ostream& out, const Order& order) {
	out << *(order.description);
	return out;
}

bool Order::validate() {
    return true;
}

void Order::execute() {
    cout << "executing" << endl;
}

////////////////////////////Deploy CLASS////////////////////////////////////
Deploy::Deploy(): Order("Deploy Default", "Deploy effect") {}
Deploy::Deploy(const Deploy& deploy_order): Order(deploy_order) {}
Deploy::~Deploy() {};
bool Deploy::validate() {
    cout << "Validating Deploy Order" << endl;
    return true;
}

void Deploy::execute() {
    if (this->validate()) {
        cout << "Executing Deploy Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

Deploy& Deploy::operator=(const Deploy& deploy_order) {
    Order::operator=(deploy_order);
    return *this;
}

ostream& operator<<(ostream& out, const Deploy& deploy) {
	out << deploy.getDescription();
	return out;
}

////////////////////////////Advance CLASS////////////////////////////////////
Advance::Advance(): Order("Advance Default", "Advance effect") {}
Advance::Advance(const Advance& adv_order): Order(adv_order) {}
Advance::~Advance() {};
bool Advance::validate() {
    cout << "Validating Advance Order" << endl;
    return true;
}

void Advance::execute() {
    if (this->validate()) {
        cout << "Executing Advance Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

Advance& Advance::operator=(const Advance& adv_order) {
    Order::operator=(adv_order);
    return *this;
}

ostream& operator<<(ostream& out, const Advance& advance) {
	out << advance.getDescription();
	return out;
}

////////////////////////////Bomb CLASS////////////////////////////////////
Bomb::Bomb(): Order("Bomb Default", "Bomb effect") {}
Bomb::Bomb(const Bomb& bomb_order): Order(bomb_order) {}
Bomb::~Bomb() {};
bool Bomb::validate() {
    cout << "Validating Bomb Order" << endl;
    return true;
}

void Bomb::execute() {
    if (this->validate()) {
        cout << "Executing Bomb Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

Bomb& Bomb::operator=(const Bomb& bomb_order) {
    Order::operator=(bomb_order);
    return *this;
}

ostream& operator<<(ostream& out, const Bomb& bomb) {
	out << bomb.getDescription();
	return out;
}

////////////////////////////Blockade CLASS////////////////////////////////////
Blockade::Blockade(): Order("Blockade Default", "Blockade effect") {}
Blockade::Blockade(const Blockade& blockade_order): Order(blockade_order) {}
Blockade::~Blockade() {};
bool Blockade::validate() {
    cout << "Validating Blockade Order" << endl;
    return true;
}

void Blockade::execute() {
    if (this->validate()) {
        cout << "Executing Blockcade Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
    
}

Blockade& Blockade::operator=(const Blockade& blockade_order) {
    Order::operator=(blockade_order);
    return *this;
}

ostream& operator<<(ostream& out, const Blockade& blockade) {
	out << blockade.getDescription();
	return out;
}

////////////////////////////Airlift CLASS////////////////////////////////////
Airlift::Airlift(): Order("Airlift Default", "Airlift effect") {}
Airlift::Airlift(const Airlift& airlift_order): Order(airlift_order) {}
Airlift::~Airlift() {};
bool Airlift::validate() {
    cout << "Validating Airlift Order" << endl;
    return true;
}

void Airlift::execute() {
    if (this->validate()) {
        cout << "Executing Airlift Order" << endl;
        cout << this->getEffect() << endl;
    }
    else {
        cout << "Invalid order cannot be executed";
    }
}

Airlift& Airlift::operator=(const Airlift& airlift_order) {
    Order::operator=(airlift_order);
    return *this;
}

ostream& operator<<(ostream& out, const Airlift& airlift) {
	out << airlift.getDescription();
	return out;
}

////////////////////////////Negotiate CLASS////////////////////////////////////
Negotiate::Negotiate(): Order("Negotiate Default", "Negotiate effect") {}
Negotiate::Negotiate(const Negotiate& negotiate_order): Order(negotiate_order) {}
Negotiate::~Negotiate() {};
bool Negotiate::validate() {
    cout << "Validating Negotiate Order" << endl;
    return true;
}

void Negotiate::execute() {
    cout << "Executing Negotiate Order" << endl;
}

Negotiate& Negotiate::operator=(const Negotiate& negotiate_order) {
    Order::operator=(negotiate_order);
    return *this;
}

ostream& operator<<(ostream& out, const Negotiate& negotiate) {
	out << negotiate.getDescription();
	return out;
}

////////////////////////////OrdersList CLASS////////////////////////////////////
OrdersList::OrdersList() {
    this->orders = new vector<Order>();
}
OrdersList::OrdersList(vector<Order>& orders) {
    this->orders = new vector<Order>(orders);
}

OrdersList::OrdersList(const OrdersList& o_list) {
    this->orders = new vector<Order>(*o_list.orders);
}

OrdersList::~OrdersList() {
    delete orders;  //might be wrong???
} 

void OrdersList::move(int currentIndex, int newIndex) {
    int order_list_size = this->orders->size();
    if (currentIndex < order_list_size && newIndex < order_list_size && currentIndex >= 0 && newIndex >= 0) {
        const Order o = this->orders->at(currentIndex);
        this->orders->erase(this->orders->begin() + currentIndex);
        this->orders->insert(this->orders->begin() + newIndex, o);
    }
    else {
        cout << "Invalid Index position specified" << endl;
    }
}

void OrdersList::remove(const int orderIndex) {
    if (orderIndex < this->orders->size()) {
        this->orders->erase(this->orders->begin() + orderIndex);
    }
    else {
        cout << "Invalid order position" << endl;
    }
}

vector<Order>& OrdersList::getOrders() const {
    return *(this->orders);
}

void OrdersList::addOrder(const Order& order) {
    this->orders->push_back(order);
}

void OrdersList::addOrder(const Order& order, int index) {
        this->orders->insert(this->orders->begin() + index, order);
}

OrdersList& OrdersList::operator=(const OrdersList& ordersList) {
    this->orders = new vector<Order>(*(ordersList.orders));
    return *this;
}

void OrdersList::operator+(Order& order) {
    this->addOrder(order);
}

ostream& operator<<(ostream& out, const OrdersList& ol) {
	out << "The OrdersList contains " << ol.orders->size() << " orders:" << endl;
    for (Order order : *(ol.orders)) {
        out << order << endl;
    }
	return out;
}

// Free function in order to test the functionality of the Orders.cpp for assignment #1.
void orders_driver() {
    cout << "\n### Running Orders driver! ###" << endl;
    OrdersList orders_list;
    Order order1;
    Deploy deploy;
    Advance advance;
    Bomb bomb;
    Blockade blockade;
    Airlift airlift;
    Negotiate negotiate;

    //add orders
    orders_list.addOrder(order1);
    orders_list.addOrder(deploy);
    orders_list.addOrder(advance);
    orders_list.addOrder(bomb);
    orders_list + blockade;
    orders_list + airlift;
    orders_list + negotiate;
    cout << orders_list << endl;
    
    //retrieve orders
    vector<Order> orders = orders_list.getOrders();

    //move orders
    orders_list.move(1, 4);
    cout << "Moved order at index 1 to index 4!" << endl; 
    cout << orders_list << endl;

    //remove orders
    orders_list.remove(3);
    cout << "Removed order at index 3!" << endl; 
    cout << orders_list << endl;

    // //validate orders
    cout << "Checking if order 1 is valid: ";
    orders[1].validate();

    // //execute orders
    orders_list.getOrders()[1].execute();

    cout << orders_list << endl;
}
