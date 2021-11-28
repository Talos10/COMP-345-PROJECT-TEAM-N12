#include "PlayerStrategies.h"

/**
 * Player Strategy class
 */
// Default constructor for Player Strategy
PlayerStrategy::PlayerStrategy() {}

// Default destructor for Player Strategy
PlayerStrategy::~PlayerStrategy() {}

/**
* Human strategy player
*/

// Default constructor
HumanPlayerStrategy::HumanPlayerStrategy() {}

// Default destructor
HumanPlayerStrategy::~HumanPlayerStrategy() {}

// Print the strategy
string HumanPlayerStrategy::printStrategy() {
    string strategy = "human strategy";
    return strategy;
}

// clone a HumanStrategyPlayer
PlayerStrategy* HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(*this);
}

void HumanPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo) {
    cout << "Issuing order from Human Player Strategy" << endl;
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toAttack(Player* player) {
    cout << "toAttack method from Human Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toDefend(Player* player) {
    cout << "toDefend method from Human Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}

/**
* Aggressive strategy player
*/

// Default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy() {}

// Default destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

// Print the strategy
string AggressivePlayerStrategy::printStrategy() {
    string strategy = "aggressive strategy";
    return strategy;
}

// clone an Aggressive Player Strategy
PlayerStrategy* AggressivePlayerStrategy::clone() const {
    return new AggressivePlayerStrategy(*this);
}

void AggressivePlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo) {
    cout << "Issuing order from Aggressive Player Strategy" << endl;
    if (get<2>(*orderInfo) == "deploy") {
        int reinforcementPool = *player->getReinforcementPool();
        Order *deploy = new Deploy(*player, *get<0>(*orderInfo), reinforcementPool);
        player->getOrdersList()->addOrder(deploy);
        cout << "**issueOrder Deploy | Player: " << *player->getPName() << " | Target territory: "
             << get<0>(*orderInfo)->getName() << " | Armies: " << reinforcementPool << endl;
//    this->log->AddSubject(*deploy);
        player->decreasePool(reinforcementPool);
    } else if (get<2>(*orderInfo) == "negotiate") {
        cout << "An Aggressive Player does not do negotiate order. It does Deploy and Advance" << endl;
    } else if (get<2>(*orderInfo) == "blockade") {
        cout << "An Aggressive Player does not do blockade order. It does Deploy and Advance" << endl;
    } else if (get<2>(*orderInfo) == "airlift" && get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
        Order *airlift = new Airlift(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                     get<0>(*orderInfo)->getNumberOfArmies() - 1);
        cout << "**issueOrder Airlift | Player: " << *player->getPName() << " | Source territory: "
             << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
             << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - 1 << endl;
        player->getOrdersList()->addOrder(airlift);
//        this->log->AddSubject(*airlift);
    } else if (get<2>(*orderInfo) == "advance") {
        //Defend Advance
        if (get<0>(*orderInfo)->getOwner() == get<1>(*orderInfo)->getOwner() &&
            get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
            Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                         get<0>(*orderInfo)->getNumberOfArmies() - 1);
            player->getOrdersList()->addOrder(advance);
            cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: "
                 << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
                 << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - 1 << endl;
        } else {  //Attack Advance
            int differenceOfArmies = get<0>(*orderInfo)->getNumberOfArmies() - get<1>(*orderInfo)->getNumberOfArmies();
            if (differenceOfArmies > 5) {
                Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                             get<0>(*orderInfo)->getNumberOfArmies() - differenceOfArmies + 5);
                player->getOrdersList()->addOrder(advance);
                cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: "
                     << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
                     << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - differenceOfArmies + 5 << endl;
            }
            else if (differenceOfArmies >= 1) {
                Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                             get<0>(*orderInfo)->getNumberOfArmies());
                player->getOrdersList()->addOrder(advance);
                cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: "
                     << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
                     << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() << endl;
            } else {
                cout
                        << "Cannot issue Advance order because numArmies source territory is less than numArmies target territory"
                        << endl;
            }
        }
//        this->log->AddSubject(*advance);
    } else if (get<2>(*orderInfo) == "bomb") {
        Order *bomb = new Bomb(*player, *get<1>(*orderInfo));
        player->getOrdersList()->addOrder(bomb);
        cout << "**issueOrder Bomb | Player: " << *player->getPName() << " | Target territory: "
             << get<1>(*orderInfo)->getName() << endl;
//        this->log->AddSubject(*bomb);
    }
}

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toAttack(Player* player) {
    cout << "toAttack method from Aggressive Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack;
    //Copy of the player's hand to keep track of which cards are going to be played
    std::map<int,int> tempHand;
    tempHand = {{0,0},{1,0},{2,0},{3,0},{4,0}};
    //Counts the number of cards for each type
    for(Card *card: *player->getHand()->getHandsCards()){
        tempHand[*card->getType()] += 1;
    }
    //Find strongest territory
    Territory* strongestTerritory = player->getTerritories()->at(0);
    for (Territory* terr : *player->getTerritories()) {
        if (terr->getNumberOfArmies() > strongestTerritory->getNumberOfArmies() && !checkIfAllNeighborsBelongToSamePlayer(terr)) {
            strongestTerritory = terr;
        }
    }
    //Attack neighbors of strongest territory
    for (Territory* neighbor: strongestTerritory->getNeighbours()) {
        if (tempHand[0] > 0 && neighbor->getNumberOfArmies() > strongestTerritory->getNumberOfArmies()) {
            toAttack.emplace_back(strongestTerritory,neighbor,"bomb");
            tempHand[0]--;
        }
        else {
            toAttack.emplace_back(strongestTerritory, neighbor,"advance");
        }
    }
    //Attack neighbors of other territories
    for (Territory* territory: *player->getTerritories()) {
        if (territory != strongestTerritory) {
            for (Territory* neighbor: territory->getNeighbours()) {
                if(neighbor->getNumberOfArmies() < territory->getNumberOfArmies() && territory->getNumberOfArmies() > 0){
                    toAttack.emplace_back(territory,neighbor,"advance");
                }
            }
        }
    }
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toDefend(Player* player) {
    cout << "toDefend method from Aggressive Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend;
    //Copy of the player's hand to keep track of which cards are going to be played
    std::map<int,int> tempHand;
    tempHand = {{0,0},{1,0},{2,0},{3,0},{4,0}};
    //Counts the number of cards for each type
    for(Card *card: *player->getHand()->getHandsCards()){
        tempHand[*card->getType()] += 1;
    }
    //Find strongest territory
    Territory* strongestTerritory = player->getTerritories()->at(0);
    for (Territory* terr : *player->getTerritories()) {
        if (terr->getNumberOfArmies() > strongestTerritory->getNumberOfArmies() && !checkIfAllNeighborsBelongToSamePlayer(terr)) {
            strongestTerritory = terr;
        }
    }
    //Deploy all of reinforcement pool on strongest territory
    if(player->getTerritories()->size() > 1){
        toDefend.emplace_back(strongestTerritory, strongestTerritory,"deploy");
    }
    //for each territory that is a neighbor of the strongest territory to an advance order towards the strongest territory
    for (Territory* territory : *player->getTerritories()) {
        bool addedAdvanceOrder = false;
        for (Territory* neighbor : territory->getNeighbours()) {
            if (neighbor == strongestTerritory) {
                toDefend.emplace_back(territory, neighbor,"advance");
                addedAdvanceOrder = true;
                break;
            }
        }
        if (tempHand[3] > 0 && !addedAdvanceOrder && territory->getNumberOfArmies() > 2) {
            toDefend.emplace_back(territory, strongestTerritory,"airlift");
            tempHand[3]--;
        }
    }
    return toDefend;
}

//Checks if all neighbors of a territory belong to the same player. If Yes, return true. Otherwise, return false.
bool AggressivePlayerStrategy::checkIfAllNeighborsBelongToSamePlayer(Territory* terr) {
    for (Territory* neighbor : terr->getNeighbours()) {
        if (neighbor->getOwner() != terr->getOwner()) {
            return false;
        }
    }
    return true;
}

/**
* Benevolent strategy player
*/

// Default constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy() {}

// Default destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

// Print the strategy
string BenevolentPlayerStrategy::printStrategy() {
    string strategy = "benevolent strategy";
    return strategy;
}

// clone a Benevolent Player Strategy
PlayerStrategy* BenevolentPlayerStrategy::clone() const {
    return new BenevolentPlayerStrategy(*this);
}

void BenevolentPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo) {
    cout << "Issuing order from Benevolent Player Strategy" << endl;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toAttack(Player* player) {
    cout << "toAttack method from Benevolent Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toDefend(Player* player) {
    cout << "toDefend method from Benevolent Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}

/**
* Neutral strategy player
*/

// Default constructor
NeutralPlayerStrategy::NeutralPlayerStrategy() {}

// Default destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

// Print the strategy
string NeutralPlayerStrategy::printStrategy() {
    string strategy = "neutral strategy";
    return strategy;
}

// clone a Neutral Player Strategy
PlayerStrategy* NeutralPlayerStrategy::clone() const {
    return new NeutralPlayerStrategy(*this);
}

void NeutralPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo) {
    cout << "Issuing order from Neutral Player Strategy" << endl;
    cout << "Neutral player does not issue any orders!" << endl;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toAttack(Player* player) {
    cout << "toAttack method from Neutral Player Strategy" << endl;
    cout << "Neutral player does not attack!" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toDefend(Player* player) {
    cout << "toDefend method from Neutral Player Strategy" << endl;
    cout << "Neutral player does not defend!" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}

/**
* Cheater strategy player
*/

// Default constructor
CheaterPlayerStrategy::CheaterPlayerStrategy() {}

// Default destructor
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

// Print the strategy
string CheaterPlayerStrategy::printStrategy() {
    string strategy = "cheater strategy";
    return strategy;
}

// clone a Cheater Player strategy
PlayerStrategy* CheaterPlayerStrategy::clone() const {
    return new CheaterPlayerStrategy(*this);
}

void CheaterPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo) {
    cout << "Issuing order from Cheater Player Strategy" << endl;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toAttack(Player* player) {
    cout << "toAttack method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toDefend(Player* player) {
    cout << "toDefend method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}