// Describe the use of the orders list class.
#ifndef COMP_345_PROJECT_TEAM_N12_ORDERS_H
#define COMP_345_PROJECT_TEAM_N12_ORDERS_H

#include <vector>
#include <iostream>
using namespace std;

class OrdersList {
private:
    vector<Order> orders;
public:
    OrdersList(){}
    OrdersList(vector<Order>& orders){}
    ~OrdersList(){} 

    void move();
    void remove();
    void getOrders();
    void addOrder();

    void operator<<(ostream& out);
    void operator=(OrdersList& ordersList);
    void operator+(Order& order);
    void operator-(Order& order);
};

class Order {
private:
    
public:
    virtual void validate();
    virtual void execute();

    void operator<<(ostream& out);
    void operator=(Order& order);
};

class Deploy : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Deploy& order);
};

class Advance : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Advance& order);
};

class Bomb : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Bomb& order);
};

class Blockade : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Blockade& order);
};

class Airlift : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Airlift& order);
};

class Negotiate : Order {
private:
    
public:
    void validate() override;
    void execute() override;

    void operator<<(ostream& out);
    void operator=(Negotiate& order);
};

#endif //COMP_345_PROJECT_TEAM_N12_ORDERS_H
