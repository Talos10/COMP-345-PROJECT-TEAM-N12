#pragma once
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class Order {
private:
    string *description;
    string *effect;

public:
    Order();
    Order(string description, string effect);
    Order(const Order& order);
    ~Order();
    virtual bool validate();
    virtual void execute();

    string getDescription() const;
    string getEffect() const;
    void setDescription(const string& description);
    void setEffect(const string& effect);

    friend ostream & operator<<(ostream& out, const Order& order);
    Order& operator=(const Order& order);
};

class Deploy : public Order {
public:
    Deploy();
    Deploy(const Deploy& order);
    ~Deploy();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Deploy& deploy);
    Deploy& operator=(const Deploy& order);
};

class Advance : public Order {
public:
    Advance();
    Advance(const Advance& order);
    ~Advance();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Advance& advance);
    Advance& operator=(const Advance& order);
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(const Bomb& order);
    ~Bomb();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Bomb& bomb);
    Bomb& operator=(const Bomb& order);
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(const Blockade& order);
    ~Blockade();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Blockade& blockade);
    Blockade& operator=(const Blockade& order);
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(const Airlift& order);
    ~Airlift();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Airlift& airlift);
    Airlift& operator=(const Airlift& order);
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(const Negotiate& order);
    ~Negotiate();
    bool validate() override;
    void execute() override;

    friend ostream & operator<<(ostream& out, const Negotiate& negotiate);
    Negotiate& operator=(const Negotiate& order);
};

class OrdersList {
private:
    vector<Order>* orders;
public:
    OrdersList();
    OrdersList(vector<Order>& orders);
    OrdersList(const OrdersList& o_list);
    ~OrdersList();

    void move(int currentIndex, int newIndex);
    void remove(int orderPosition);
    vector<Order>& getOrders() const;
    void addOrder(const Order& order);
    void addOrder(const Order& order, int index);

    friend ostream & operator<<(ostream& out, const OrdersList& ol);
    OrdersList& operator=(const OrdersList& ordersList);
    void operator+(Order& order);
};

// Free function in order to test the functionality of the Orders.cpp for assignment #1.
void orders_driver();