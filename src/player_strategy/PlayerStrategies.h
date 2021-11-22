#pragma once

#include "orders/orders.h"

class Territory;
class Order;

class PlayerStrategy {
public:
    // Default constructor
    PlayerStrategy();

    // Default destructor
    ~PlayerStrategy();
    /**
     * Method that takes an Order parameter will issue an order depending on the chosen player strategy
     * @param order
     */

    // clones a PlayerStrategy clone
    virtual PlayerStrategy* clone() const = 0;

    virtual void issueOrder(Order* order) = 0;
    /**
     * Method that takes none arguments, attacks a territory and returns nothing
     * @return
     */
    virtual vector<tuple<Territory*,Territory*,string>> toAttack() = 0;
    /**
     * Method that takes none arguments, defends a territory and returns nothing
     * @return
     */
    virtual vector<tuple<Territory*,Territory*,string>> toDefend() = 0;

    /**
     * Method that returns the player strategy as a string
     * @return
     */
     virtual string printStrategy() = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
private:
    PlayerStrategy* clone() const override;
public:
    // Default constructor
    HumanPlayerStrategy();

    // Destructor
    ~HumanPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the human player type
    void issueOrder(Order* order) override;

    // toAttack method for the human player
    vector<tuple<Territory*,Territory*,string>> toAttack() override;

    // toDefend method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toDefend() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
private:
    PlayerStrategy* clone() const override;
public:
    // Default constructor
    AggressivePlayerStrategy();

    // Destructor
    ~AggressivePlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the aggressive player type
    void issueOrder(Order* order) override;

    // toAttack method for the aggressive player
    vector<tuple<Territory*,Territory*,string>> toAttack() override;

    // toDefend method for the aggressive player
    vector<tuple<Territory*,Territory*,string>> toDefend() override;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
private:
    PlayerStrategy* clone() const override;
public:
    // Default constructor
    BenevolentPlayerStrategy();

    // Destructor
    ~BenevolentPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the benevolent player type
    void issueOrder(Order* order) override;

    // toAttack method for the benevolent player
    vector<tuple<Territory*,Territory*,string>> toAttack() override;

    // toDefend method for the benevolent player
    vector<tuple<Territory*,Territory*,string>> toDefend() override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
private:
    PlayerStrategy* clone() const override;
public:
    // Default constructor
    NeutralPlayerStrategy();

    // Destructor
    ~NeutralPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the neutral player type
    void issueOrder(Order* order) override;

    // toAttack method for the neutral player
    vector<tuple<Territory*,Territory*,string>> toAttack() override;

    // toDefend method for the neutral player
    vector<tuple<Territory*,Territory*,string>> toDefend() override;
};

class CheaterPlayerStrategy: public PlayerStrategy {
private:
    PlayerStrategy* clone() const override;
public:
    // Default constructor
    CheaterPlayerStrategy();

    // Destructor
    ~CheaterPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the cheater player type
    void issueOrder(Order* order) override;

    // toAttack method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toAttack() override;

    // toDefend method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toDefend() override;
};


