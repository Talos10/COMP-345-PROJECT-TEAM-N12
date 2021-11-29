#pragma once

#include "orders/orders.h"

class Territory;
class Order;
class Player;

class PlayerStrategy {
public:
    // Default constructor
    PlayerStrategy();

    // Default destructor
    ~PlayerStrategy();

    // clones a PlayerStrategy clone
    [[nodiscard]] virtual PlayerStrategy *clone() const = 0;

    /**
     * Method that takes an Order parameter will issue an order depending on the chosen player strategy
     * @param order the order to be issued
     */
    virtual void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) = 0;

    /**
     * Method that takes none arguments, attacks a territory and returns nothing
     * @return the territories to be attacked
     */
    virtual vector<tuple<Territory *, Territory *, string>> toAttack(Player* player) = 0;

    /**
     * Method that takes none arguments, defends a territory and returns nothing
     * @return the territories to be defended
     */
    virtual vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) = 0;

    /**
     * Method that returns the player strategy as a string
     * @return
     */
     virtual string printStrategy() = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
private:
    // clones a HumanPlayerStrategy clone
    [[nodiscard]] PlayerStrategy *clone() const override;

public:
    // Default constructor
    HumanPlayerStrategy();

    // Destructor
    ~HumanPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the human player type
    void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) override;

    // toAttack method for the human player
    vector<tuple<Territory*,Territory*,string>> toAttack(Player* player) override;

    // toDefend method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
private:
    // clones a AggressivePlayerStrategy clone
    [[nodiscard]] PlayerStrategy *clone() const override;

public:
    // Default constructor
    AggressivePlayerStrategy();

    // Destructor
    ~AggressivePlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the aggressive player type
    void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) override;

    // toAttack method for the aggressive player
    vector<tuple<Territory*,Territory*,string>> toAttack(Player* player) override;

    // toDefend method for the aggressive player
    vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) override;

    //Checks if all neighbors of a territory belong to the same player (owner)
    bool checkIfAllNeighborsBelongToSamePlayer(Territory* terr);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
private:
    // clones a BenevolentPlayerStrategy clone
    [[nodiscard]] PlayerStrategy *clone() const override;

public:
    // Default constructor
    BenevolentPlayerStrategy();

    // Destructor
    ~BenevolentPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the benevolent player type
    void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) override;

    // toAttack method for the benevolent player
    vector<tuple<Territory*,Territory*,string>> toAttack(Player* player) override;

    // toDefend method for the benevolent player
    vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) override;
};

class NeutralPlayerStrategy : public PlayerStrategy {
private:
    // clones a NeutralPlayerStrategy clone
    [[nodiscard]] PlayerStrategy *clone() const override;

public:
    // Default constructor
    NeutralPlayerStrategy();

    // Destructor
    ~NeutralPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the neutral player type
    void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) override;

    // toAttack method for the neutral player
    vector<tuple<Territory*,Territory*,string>> toAttack(Player* player) override;

    // toDefend method for the neutral player
    vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) override;
};

class CheaterPlayerStrategy : public PlayerStrategy {
private:
    // clones a CheaterPlayerStrategy clone
    [[nodiscard]] PlayerStrategy *clone() const override;

public:
    // Default constructor
    CheaterPlayerStrategy();

    // Destructor
    ~CheaterPlayerStrategy();

    // Output operator
    string printStrategy() override;

    // issueOrder method for the cheater player type
    void issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) override;

    // toAttack method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toAttack(Player* player) override;

    // toDefend method for the cheater player
    vector<tuple<Territory*,Territory*,string>> toDefend(Player* player) override;
};


