#pragma once
#include "observer/logging_observer.h"
#include "game_log/log_observer.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

// This class implements a generic order.
class Order : public Subject, public ILoggable {
private:
    //A string which describes the order.
    string *description;

    //A string which contains the effect of the order.
    string *effect;

public:
    //Default constructor which initializes a generic order.
    Order();

    //A parameterized constructor which initializes an order with the provided description and effect.
    Order(const string& description, const string& effect);

    //Copy constructor
    Order(const Order& order);

    //Desctructor
    ~Order();

    //Checks if an order is valid.
    virtual bool validate();

    //Executes an order if it is valid.
    virtual void execute();

    //Getter for the description of the order
    string* getDescription() const;

    //Getter for the effect of the order
    string* getEffect() const;

    //Setter for the description of the order
    void setDescription(const string& description);

    //Setter for the effect of the order
    void setEffect(const string& effect);

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Order& order);

    //Defining the assignment operator
    Order& operator=(const Order& order);

    // Override class from ILoggable
    virtual string stringToLog() const override;
};

// This class implements a Deploy order.
class Deploy : public Order {
public:
    //Default constructor
    Deploy();

    //Copy constructor
    Deploy(const Deploy& order);

    //Destructor
    ~Deploy();

    //Checks if a Deploy order is valid
    bool validate() override;

    //Executes a Deploy order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Deploy& deploy);

    //Defining the assignment operator
    Deploy& operator=(const Deploy& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements an Advance order.
class Advance : public Order {
public:
    //Default constructor
    Advance();

    //Copy constructor
    Advance(const Advance& order);

    //Destructor
    ~Advance();

    //Checks if an Advance order is valid
    bool validate() override;

    //Executes an Advance order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Advance& advance);

    //Defining the assignment operator
    Advance& operator=(const Advance& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements a Bomb order.
class Bomb : public Order {
public:
    //Default constructor
    Bomb();

    //Copy constructor
    Bomb(const Bomb& order);

    //Destructor
    ~Bomb();

    //Checks if a Bomb order is valid
    bool validate() override;

    //Executes a Bomb order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Bomb& bomb);

    //Defining the assignment operator
    Bomb& operator=(const Bomb& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements a Blockade order.
class Blockade : public Order {
public:
    //Default constructor
    Blockade();

    //Copy constructor
    Blockade(const Blockade& order);

    //Destructor
    ~Blockade();

    //Checks if a Blockade order is valid
    bool validate() override;

    //Executes a Blockade order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Blockade& blockade);

    //Defining the assignment operator
    Blockade& operator=(const Blockade& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements an Airlift order.
class Airlift : public Order {
public:
    //Default constructor
    Airlift();

    //Copy constructor
    Airlift(const Airlift& order);

    //Destructor
    ~Airlift();

    //Checks if an Airlift order is valid
    bool validate() override;

    //Executes an Airlift order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Airlift& airlift);

    //Defining the assignment operator
    Airlift& operator=(const Airlift& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements a Negotiate order.
class Negotiate : public Order {
public:
    //Default constructor
    Negotiate();

    //Copy constructor
    Negotiate(const Negotiate& order);

    //Destructor
    ~Negotiate();

    //Checks if a Negotiate order is valid
    bool validate() override;

    //Executes a Negotiate order
    void execute() override;

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Negotiate& negotiate);

    //Defining the assignment operator
    Negotiate& operator=(const Negotiate& order);

    // Override class from Order
    string stringToLog() const override;
};

// This class implements an OrdersList which contains the orders created.
class OrdersList : public Subject, public ILoggable {
private:
    vector<Order*>* orders;
public:
    //Default constructor
    OrdersList();

    //Copy constructor
    OrdersList(const OrdersList& o_list);

    //Destructor
    ~OrdersList();

    //Move an Order in the vector to a new index by providing its current index and the index it should be moved to
    void move(int currentIndex, int newIndex);

    //Remove an order in the vector by specifying its respective index
    void remove(int orderPosition);

    //Getter for the vector of orders
    vector<Order*>* getOrders() const;

    //Add an order to the vector by providing an Order
    void addOrder(Order* order);

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const OrdersList& ol);

    //Defining the assignment operator
    OrdersList& operator=(const OrdersList& ordersList);

    //Defining the addition operator
    void operator+(Order* order);

    // Override pure virtual method from IlLoggable
    string stringToLog() const override;
};

// Free function in order to test the functionality of the Orders.cpp for assignment #1.
void orders_driver();