#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "map/map.h"
using namespace std;

//Forward declaration
class Player;
class Territory;

// This class implements a generic order.
class Order {
private:
    //A string which describes the order.
    string *description;

    //A string which contains the effect of the order.
    string *effect;

    //Player that issues the order
    Player* issuingPlayer;

public:
    //Default constructor which initializes a generic order.
    Order();

    //A parameterized constructor which initializes an order with the provided description, effect and issuing player
    Order(const string& description, const string& effect, Player& issuingPlayer);

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

    //Getter for the issuing player of the order
    Player* getIssuingPlayer() const;

    //Setter for the description of the order
    void setDescription(const string& description);

    //Setter for the effect of the order
    void setEffect(const string& effect);

    //Setter for the issuing player of the order
    void setIssuingPlayer(Player& issuingPlayer);

    //Defining the output operator
    friend ostream & operator<<(ostream& out, const Order& order);

    //Defining the assignment operator
    Order& operator=(const Order& order);
};

// This class implements a Deploy order.
class Deploy : public Order {
private:
    Territory* targetTerritory;
    int numArmies;
public:
    //Default constructor
    Deploy();

    //Parameterized Constructor
    Deploy(Player& issuingPlayer, Territory& targetTerritory, int numArmies);

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
};

// This class implements an Advance order.
class Advance : public Order {
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int numArmies;
public:
    //Default constructor
    Advance();

    //Parameterized Constructor
    Advance(Player& issuingPlayer, Territory& sourceTerritory, Territory& targetTerritory, int numArmies);

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
};

// This class implements a Bomb order.
class Bomb : public Order {
private:
    Territory* targetTerritory;
public:
    //Default constructor
    Bomb();

    //Parameterized Constructor
    Bomb(Player& issuingPlayer, Territory& targetTerritory);

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
};

// This class implements a Blockade order.
class Blockade : public Order {
private:
    Territory* targetTerritory;
public:
    //Default constructor
    Blockade();

    //Parameterized Constructor
    Blockade(Player& issuingPlayer, Territory& targetTerritory);

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
};

// This class implements an Airlift order.
class Airlift : public Order {
private:
    Territory* sourceTerritory;
    Territory* targetTerritory;
    int numArmies;
public:
    //Default constructor
    Airlift();

    //Parameterized Constructor
    Airlift(Player& issuingPlayer, Territory& sourceTerritory, Territory& targetTerritory, int numArmies);

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
};

// This class implements a Negotiate order.
class Negotiate : public Order {
private:
    Player* enemyPlayer;
public:
    //Default constructor
    Negotiate();

    //Parameterized Constructor
    Negotiate(Player& issuingPlayer, Player& enemyPlayer);

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
};

// This class implements an OrdersList which contains the orders created.
class OrdersList {
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
};

// Free function in order to test the functionality of the Orders.cpp for assignment #1.
void orders_driver();