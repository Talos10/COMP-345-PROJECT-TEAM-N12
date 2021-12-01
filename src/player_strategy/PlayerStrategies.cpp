#include "PlayerStrategies.h"
#include "game_engine/game_engine.h"

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
PlayerStrategy *HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(*this);
}

void HumanPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) {
    cout << "Issuing order from Human Player Strategy" << endl;
    if (get<2>(*orderInfo) == "deploy") {
        cout << "Issuing deploy order!" << endl;
        int reinforcementPool = *player->getReinforcementPool();
        int nbrDeploy;
        bool repeat = true;

        do {
            cout << "Issuing a deploy order. Your current reinforcement pool is " << reinforcementPool << endl;
            cout << "Please enter the number of armies to deploy on " << get<0>(*orderInfo)->getName() << endl;
            cin >> nbrDeploy;

            if (nbrDeploy <= reinforcementPool) {
                player->decreasePool(nbrDeploy);
                repeat = false;
            } else if (nbrDeploy < 0 || nbrDeploy > reinforcementPool) {
                cout << "Cannot deploy this amount of armies: " << nbrDeploy << ". Please try again." << endl;
            }
        } while (repeat);

        Order *deploy = new Deploy(*player, *get<0>(*orderInfo), nbrDeploy);
        player->getOrdersList()->addOrder(deploy);
        cout << "**issueOrder Deploy | Player: " << *player->getPName() << " | Target territory: "
             << get<0>(*orderInfo)->getName() << " | Armies: " << nbrDeploy << endl;
        log.AddSubject(*deploy);

    } else if (get<2>(*orderInfo) == "negotiate") {
        cout << "Issuing negotiate order!" << endl;
        Order *negotiate = new Negotiate(*player, *get<1>(*orderInfo)->getOwner());
        player->getOrdersList()->addOrder(negotiate);
        log.AddSubject(*negotiate);

    } else if (get<2>(*orderInfo) == "blockade") {
        cout << "Issuing blockade order!" << endl;
        Order *blockade = new Blockade(*player, *GameEngine::getNeutralPlayer(), *get<1>(*orderInfo));
        player->getOrdersList()->addOrder(blockade);
        log.AddSubject(*blockade);
        cout << "**issueOrder Blockade | Player: " << *player->getPName() << " | Neutral player: "
             << *GameEngine::getNeutralPlayer()->getPName() << " | Target territory: " << get<1>(*orderInfo)->getName()
             << endl;

    } else if (get<2>(*orderInfo) == "airlift") {
        int nbrAirlift;
        bool repeat = true;

        if (get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
            do {
                cout << "Issuing an airlift order. Your current number of armies on territory "
                     << get<0>(*orderInfo)->getName() << " is " << get<0>(*orderInfo)->getNumberOfArmies() << endl;
                cout << "Please enter the number of armies to airlift to territory " << get<1>(*orderInfo)->getName()
                     << endl;
                cin >> nbrAirlift;

                if (nbrAirlift < get<0>(*orderInfo)->getNumberOfArmies()) {
                    repeat = false;
                } else if (nbrAirlift < 0 || nbrAirlift > get<0>(*orderInfo)->getNumberOfArmies()) {
                    cout << "Cannot airlift this amount of armies: " << nbrAirlift
                         << ". The number is either negative or more than the current amount of armies on the source territory. Please try again."
                         << endl;
                } else {
                    cout << "Cannot airlift this amount of armies: " << nbrAirlift
                         << " (must leave at least one army on source territory). Please try again." << endl;
                }
            } while (repeat);

            Order *airlift = new Airlift(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                         nbrAirlift);
            cout << "**issueOrder Airlift | Player: " << *player->getPName() << " | Source territory: "
                 << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
                 << " | Armies left on source territory: " << get<0>(*orderInfo)->getNumberOfArmies() - nbrAirlift
                 << endl;
            player->getOrdersList()->addOrder(airlift);
            log.AddSubject(*airlift);
        } else {
            cout << "Cannot issue airlift order because numArmies source territory " << get<0>(*orderInfo)->getName()
                 << ", armies = " << get<0>(*orderInfo)->getNumberOfArmies() << " is less than 2" << endl;
        }
    } else if (get<2>(*orderInfo) == "advance") {
        int nbrAdvance;
        bool repeat = true;

        if (get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
            do {
                cout << "Issuing an advance order." << endl;
                cout << "Your current number of armies on the source territory " << get<0>(*orderInfo)->getName()
                     << " is "
                     << get<0>(*orderInfo)->getNumberOfArmies() << endl;

                if (get<0>(*orderInfo)->getOwner() == get<1>(*orderInfo)->getOwner()) {
                    cout << "The current number of armies on the FRIENDLY target territory "
                         << get<1>(*orderInfo)->getName() << " is " << get<1>(*orderInfo)->getNumberOfArmies() << endl;
                } else {
                    cout << "The current number of armies on the HOSTILE target territory "
                         << get<1>(*orderInfo)->getName() << " is " << get<1>(*orderInfo)->getNumberOfArmies() << endl;
                }

                cout << "Please enter the number of armies to advance from territory " << get<0>(*orderInfo)->getName()
                     << " to territory " << get<1>(*orderInfo)->getName() << endl;
                cin >> nbrAdvance;

                if (nbrAdvance < get<0>(*orderInfo)->getNumberOfArmies()) {
                    repeat = false;
                } else if (nbrAdvance < 0 || nbrAdvance > get<0>(*orderInfo)->getNumberOfArmies()) {
                    cout << "Cannot advance this amount of armies: " << nbrAdvance
                         << ". The number is either negative or more than the current amount of armies on the source territory. Please try again."
                         << endl;
                } else {
                    cout << "Cannot advance this amount of armies: " << nbrAdvance
                         << " (must leave at least one army on source territory). Please try again." << endl;
                }
            } while (repeat);

            Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),
                                         nbrAdvance);
            player->getOrdersList()->addOrder(advance);
            log.AddSubject(*advance);
            cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: "
                 << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName()
                 << " , owner: " << *get<1>(*orderInfo)->getOwner()->getPName()
                 << " | Armies left on source territory: " << get<0>(*orderInfo)->getNumberOfArmies() - nbrAdvance
                 << endl;
        } else {
            cout << "Cannot issue advance order because numArmies source territory " << get<0>(*orderInfo)->getName()
                 << ", armies = " << get<0>(*orderInfo)->getNumberOfArmies() << " is less than 2" << endl;
        }
    } else if (get<2>(*orderInfo) == "bomb") {
        cout << "Issuing a bomb order." << endl;
        Order *bomb = new Bomb(*player, *get<1>(*orderInfo));
        player->getOrdersList()->addOrder(bomb);
        cout << "**issueOrder Bomb | Player: " << *player->getPName() << " | Target territory: "
             << get<1>(*orderInfo)->getName() << endl;
        log.AddSubject(*bomb);
    }
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toAttack(Player *player) {
    cout << "toAttack method from Human Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack;
    //Copy of the player's hand to keep track of which cards are going to be played
    std::map<int, pair<string, int>> tempHand;
    int sourceTerritoryIndex;
    int targetTerritoryIndex;
    string targetTerritoryInput;
    int orderType;
    bool shouldContinue = true;
    bool chooseTerritoryAgain = true;
    string orderName;
    tempHand = {{0, pair("bomb", 0)},
                {1, pair("reinforcement", 0)},
                {2, pair("blockade", 0)},
                {3, pair("airlift", 0)},
                {4, pair("negotiate", 0)}};
    //Counts the number of cards for each type
    for (Card *card: *player->getHand()->getHandsCards()) {
        tempHand[*card->getType()].second += 1;
    }

    do {
        chooseTerritoryAgain = true;
        cout << "Here are your current possible attack order types:\n" << endl;

        cout << "Attack order type #" << 0 << ": Attack order type name: " << tempHand[0].first
             << " | Number of such cards in hand: " << tempHand[0].second << endl;
        cout << "Attack order type #" << 1 << ": Attack order type name: " << "advance" << endl;

        cout << "Enter the wanted attack order type (enter -1 if you wish to move on):" << endl;

        cin >> orderType;

        if (orderType == -1) {
            shouldContinue = false;
            continue;
        } else if (orderType < 0 || orderType > 1) {
            cout << "The order type number " << orderType << " doesn't exist! Please enter a number between 0-1!"
                 << endl;
            continue;
        }
        switch (orderType) {
            case 1:
                orderName = "advance";
                break;
            default:
                if (tempHand[orderType].second == 0) {
                    cout << "You have no cards of type " << tempHand[orderType].first << " to play in your hand!"
                         << endl;
                    continue;
                }
                orderName = tempHand[orderType].first;
                tempHand[orderType].second -= 1;
        }

        cout << "\nYou selected a attack order of type " << orderName << "\n" << endl;

        do {
            cout
                    << "-Choose two territories from the list below. Type the first territory id, hit enter, and then type the second territory id and hit enter."
                    << endl;
            cout
                    << "-For advance and bomb, the second territory should be neighbouring the first one (and should be a hostile territory)."
                    << endl;
            cout
                    << "-For advance and bomb, the second id should be formatted like so \"2-5\"."
                    << endl;

            cout
                    << "This means that the second territory is territory with id 5 which is a neighbour of territory with id 2.\n"
                    << endl;


            printPlayerTerritories(player);

            cout << "\nEnter source territory id:" << endl;
            cin >> sourceTerritoryIndex;

            cout << "\nEnter target territory id:" << endl;
            cin >> targetTerritoryInput;

            try {
                Territory *sourceTerritory = player->getTerritories()->at(sourceTerritoryIndex);
                Territory *targetTerritory = nullptr;
                auto end = targetTerritoryInput.find('-');
                if (end == string::npos) {
                    targetTerritoryIndex = stoi(targetTerritoryInput);
                    targetTerritory = player->getTerritories()->at(targetTerritoryIndex);
                } else {
                    int outterTer = stoi(targetTerritoryInput.substr(0, end));
                    int innerTer = stoi(targetTerritoryInput.substr(end + 1, targetTerritoryInput.size()));
                    int count = 0;
                    for (auto const &entry: player->getTerritories()->at(outterTer)->getNeighbours()) {
                        if (count == innerTer) {
                            targetTerritory = entry;
                            break;
                        }
                        count++;
                    }
                }

                if (targetTerritory == nullptr) {
                    throw out_of_range("No neighbour territory found with " + targetTerritoryInput);
                }

                toAttack.emplace_back(sourceTerritory, targetTerritory, orderName);
                chooseTerritoryAgain = false;

            } catch (out_of_range &e) {
                cerr << "Incorrect first or second territory id! Choose two territories again." << endl;
            }
        } while (chooseTerritoryAgain);

    } while (shouldContinue);
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> HumanPlayerStrategy::toDefend(Player *player) {
    cout << "toDefend method from Human Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend;
    //Copy of the player's hand to keep track of which cards are going to be played
    std::map<int, pair<string, int>> tempHand;
    int sourceTerritoryIndex;
    int targetTerritoryIndex;
    string targetTerritoryInput;
    int orderType;
    bool shouldContinue = true;
    bool chooseTerritoryAgain = true;
    string orderName;
    tempHand = {{0, pair("bomb", 0)},
                {1, pair("reinforcement", 0)},
                {2, pair("blockade", 0)},
                {3, pair("airlift", 0)},
                {4, pair("negotiate", 0)}};
    //Counts the number of cards for each type
    for (Card *card: *player->getHand()->getHandsCards()) {
        tempHand[*card->getType()].second += 1;
    }

    do {
        chooseTerritoryAgain = true;
        cout << "Here are your current possible defend order types:\n" << endl;

        cout << "Defend order type #" << 0 << ": Defend order type name: " << "deploy" << endl;
        cout << "Defend order type #" << 1 << ": Defend order type name: " << tempHand[1].first
             << " | Number of such cards in hand: " << tempHand[1].second << endl;
        cout << "Defend order type #" << 2 << ": Defend order type name: " << tempHand[2].first
             << " | Number of such cards in hand: " << tempHand[2].second << endl;
        cout << "Defend order type #" << 3 << ": Defend order type name: " << tempHand[3].first
             << " | Number of such cards in hand: " << tempHand[3].second << endl;
        cout << "Defend order type #" << 4 << ": Defend order type name: " << tempHand[4].first
             << " | Number of such cards in hand: " << tempHand[4].second << endl;
        cout << "Defend order type #" << 5 << ": Defend order type name: " << "advance" << endl;

        cout << "Enter the wanted defend order type (enter -1 if you wish to move on):" << endl;

        cin >> orderType;

        if (orderType == -1) {
            shouldContinue = false;
            continue;
        } else if (orderType < 0 || orderType > 5) {
            cout << "The order type number " << orderType << " doesn't exist! Please enter a number between 0-5!"
                 << endl;
            continue;
        }
        switch (orderType) {
            case 0:
                orderName = "deploy";
                break;
            case 5:
                orderName = "advance";
                break;
            default:
                if (tempHand[orderType].second == 0) {
                    cout << "You have no cards of type " << tempHand[orderType].first << " to play in your hand!"
                         << endl;
                    continue;
                }
                orderName = tempHand[orderType].first;
                tempHand[orderType].second -= 1;
        }

        cout << "\nYou selected a defend order of type " << orderName << "\n" << endl;

        do {
            cout
                    << "Choose two territories from the list below. Type the first territory id, hit enter, and then type the second territory id and hit enter."
                    << endl;
            cout << "For deploy and blockade, the same territory should be entered." << endl;
            cout
                    << "For negotiate, the second territory should be neighbouring the first one (and should be a hostile territory)."
                    << endl;
            cout
                    << "For negotiate, the second id should be formatted like so \"2-5\" which means that the second territory is territory with id 5 which is a neighbour of territory with id 2.\n"
                    << endl;

            printPlayerTerritories(player);

            cout << "\nEnter source territory id:" << endl;
            cin >> sourceTerritoryIndex;

            cout << "\nEnter target territory id:" << endl;
            cin >> targetTerritoryInput;

            try {
                Territory *sourceTerritory = player->getTerritories()->at(sourceTerritoryIndex);
                Territory *targetTerritory = nullptr;
                auto end = targetTerritoryInput.find('-');
                if (end == string::npos) {
                    targetTerritoryIndex = stoi(targetTerritoryInput);
                    targetTerritory = player->getTerritories()->at(targetTerritoryIndex);
                } else {
                    int outterTer = stoi(targetTerritoryInput.substr(0, end));
                    int innerTer = stoi(targetTerritoryInput.substr(end + 1, targetTerritoryInput.size()));
                    int count = 0;
                    for (auto const &entry: player->getTerritories()->at(outterTer)->getNeighbours()) {
                        if (count == innerTer) {
                            targetTerritory = entry;
                            break;
                        }
                        count++;
                    }
                }

                if (targetTerritory == nullptr) {
                    throw out_of_range("No neighbour territory found with " + targetTerritoryInput);
                }

                toDefend.emplace_back(sourceTerritory, targetTerritory, orderName);
                chooseTerritoryAgain = false;

            } catch (out_of_range &e) {
                cerr << e.what() << endl;
                cerr << "Incorrect first or second territory id! Choose two territories again." << endl;
            }
        } while (chooseTerritoryAgain);

    } while (shouldContinue);

    return toDefend;
}

void HumanPlayerStrategy::printPlayerTerritories(const Player *player) {
    for (int i = 0; i < player->getTerritories()->size(); i++) {
        cout << "\nTerritory number #" << i << ": " << player->getTerritories()->at(i) << endl;

        int count = 0;
        for (auto const &entry: player->getTerritories()->at(i)->getNeighbours()) {
            cout << "\t-Neighbour Territory number #" << count << ": " << entry << endl;
            count++;
        }
    }
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
PlayerStrategy *AggressivePlayerStrategy::clone() const {
    return new AggressivePlayerStrategy(*this);
}

void AggressivePlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) {
    cout << "Issuing order from Aggressive Player Strategy" << endl;
    if (get<2>(*orderInfo) == "deploy") {
        int reinforcementPool = *player->getReinforcementPool();
        Order *deploy = new Deploy(*player, *get<0>(*orderInfo), reinforcementPool);
        player->getOrdersList()->addOrder(deploy);
        cout << "**issueOrder Deploy | Player: " << *player->getPName() << " | Target territory: " << get<0>(*orderInfo)->getName() << " | Armies: " << reinforcementPool << endl;
        log.AddSubject(*deploy);
        player->decreasePool(reinforcementPool);
    }
    else if (get<2>(*orderInfo) == "negotiate") {
        cout << "An Aggressive Player does not do negotiate order. It does Deploy and Advance" << endl;
    }
    else if (get<2>(*orderInfo) == "blockade") {
        cout << "An Aggressive Player does not do blockade order. It does Deploy and Advance" << endl;
    }
    else if (get<2>(*orderInfo) == "airlift" && get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
        Order *airlift = new Airlift(*player, *get<0>(*orderInfo), *get<1>(*orderInfo), get<0>(*orderInfo)->getNumberOfArmies() - 1);
        cout << "**issueOrder Airlift | Player: " << *player->getPName() << " | Source territory: " << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName() << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - 1 << endl;
        player->getOrdersList()->addOrder(airlift);
        log.AddSubject(*airlift);
    }
    else if (get<2>(*orderInfo) == "advance") {
        //Defend Advance
        if (get<0>(*orderInfo)->getOwner() == get<1>(*orderInfo)->getOwner() && get<0>(*orderInfo)->getNumberOfArmies() >= 2) {
            Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),get<0>(*orderInfo)->getNumberOfArmies() - 1);
            player->getOrdersList()->addOrder(advance);
            cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName() << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - 1 << endl;
            log.AddSubject(*advance);
        }
        else {  //Attack Advance
            int differenceOfArmies = get<0>(*orderInfo)->getNumberOfArmies() - get<1>(*orderInfo)->getNumberOfArmies();
            if (differenceOfArmies > 5) {
                Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo), get<0>(*orderInfo)->getNumberOfArmies() - differenceOfArmies + 5);
                player->getOrdersList()->addOrder(advance);
                cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName() << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() - differenceOfArmies + 5 << endl;
                log.AddSubject(*advance);
            }
            else if (differenceOfArmies >= 1) {
                Order *advance = new Advance(*player, *get<0>(*orderInfo), *get<1>(*orderInfo),get<0>(*orderInfo)->getNumberOfArmies());
                player->getOrdersList()->addOrder(advance);
                cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(*orderInfo)->getName() << " | Target territory: " << get<1>(*orderInfo)->getName() << " | Armies: " << get<0>(*orderInfo)->getNumberOfArmies() << endl;
                log.AddSubject(*advance);
            }
            else {
                cout << "Cannot issue Advance order because numArmies source territory "
                     << get<0>(*orderInfo)->getName() << ", armies = " << get<0>(*orderInfo)->getNumberOfArmies()
                     << " is less than or equal to the numArmies target territory " << get<1>(*orderInfo)->getName()
                     << ", armies = " << get<1>(*orderInfo)->getNumberOfArmies() << endl;
            }
        }
    }
    else if (get<2>(*orderInfo) == "bomb") {
        Order *bomb = new Bomb(*player, *get<1>(*orderInfo));
        player->getOrdersList()->addOrder(bomb);
        cout << "**issueOrder Bomb | Player: " << *player->getPName() << " | Target territory: " << get<1>(*orderInfo)->getName() << endl;
        log.AddSubject(*bomb);
    }
}

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toAttack(Player *player) {
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

vector<tuple<Territory *, Territory *, string>> AggressivePlayerStrategy::toDefend(Player *player) {
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
    if (!player->getTerritories()->empty()) {
        toDefend.emplace_back(strongestTerritory, strongestTerritory, "deploy");
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
        if (tempHand[3] > 0 && !addedAdvanceOrder && territory->getNumberOfArmies() > 2 && territory != strongestTerritory) {
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
PlayerStrategy *BenevolentPlayerStrategy::clone() const {
    return new BenevolentPlayerStrategy(*this);
}

void BenevolentPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) {
    cout << "Issuing order from Benevolent Player Strategy" << endl;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toAttack(Player *player) {
    cout << "toAttack method from Benevolent Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> BenevolentPlayerStrategy::toDefend(Player *player) {
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
PlayerStrategy *NeutralPlayerStrategy::clone() const {
    return new NeutralPlayerStrategy(*this);
}

void NeutralPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) {
    cout << "Issuing order from Neutral Player Strategy" << endl;
    cout << "Neutral player does not issue any orders!" << endl;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toAttack(Player *player) {
    cout << "toAttack method from Neutral Player Strategy" << endl;
    cout << "Neutral player does not attack!" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> NeutralPlayerStrategy::toDefend(Player *player) {
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
PlayerStrategy *CheaterPlayerStrategy::clone() const {
    return new CheaterPlayerStrategy(*this);
}

void CheaterPlayerStrategy::issueOrder(Player *player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log) {
    cout << "Issuing order from Cheater Player Strategy" << endl;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toAttack(Player *player) {
    cout << "toAttack method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toAttack{};
    return toAttack;
}

vector<tuple<Territory *, Territory *, string>> CheaterPlayerStrategy::toDefend(Player *player) {
    cout << "toDefend method from Cheater Player Strategy" << endl;
    vector<tuple<Territory *, Territory *, string>> toDefend{};
    return toDefend;
}