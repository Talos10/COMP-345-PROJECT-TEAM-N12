//
// Created by razva on 9/23/2021.
//

#include "Orders.h"

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
