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

void HumanPlayerStrategy::issueOrder(Order *order) {
    cout << "Issuing order from Human Player Strategy" << endl;
    cout << order;
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toAttack() {
    cout << "toAttack method from Human Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toDefend() {
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

void AggressivePlayerStrategy::issueOrder(Order *order) {
    cout << "Issuing order from Aggressive Player Strategy" << endl;
    cout << order;
}

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toAttack() {
    cout << "toAttack method from Aggressive Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toDefend() {
    cout << "toDefend method from Aggressive Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
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

void BenevolentPlayerStrategy::issueOrder(Order *order) {
    cout << "Issuing order from Benevolent Player Strategy" << endl;
    cout << order;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toAttack() {
    cout << "toAttack method from Benevolent Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toDefend() {
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

void NeutralPlayerStrategy::issueOrder(Order *order) {
    cout << "Issuing order from Neutral Player Strategy" << endl;
    cout << order;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toAttack() {
    cout << "toAttack method from Neutral Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toDefend() {
    cout << "toDefend method from Neutral Player Strategy" << endl;
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

void CheaterPlayerStrategy::issueOrder(Order *order) {
    cout << "Issuing order from Cheater Player Strategy" << endl;
    cout << order;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toAttack() {
    cout << "toAttack method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toDefend() {
    cout << "toDefend method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}