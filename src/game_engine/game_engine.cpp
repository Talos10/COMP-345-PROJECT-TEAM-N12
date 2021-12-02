#include "game_engine.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <math.h>
#include <chrono>
#include <algorithm>
#include <random>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;

// The implementation file of the GameEngine class.

// One param constructor which initializes all the maps, the current game state, and the mode in which the game
// start-up commands will be read (from console or file).
GameEngine::GameEngine(const string &readMode) {
    // Add the Observer and attach the game engine
    log = new LogObserver();
    log->AddSubject(*this);

    // Map and state initializations
    currentState = new string("start");
    stateMap = new std::map<string, vector<string> *>{};
    descriptionMap = new std::map<string, tuple<string, int, string>>{};
    functionMap = new std::map<string, pair<Game_Engine_Mem_Fn, string>>{};
    commandReadMode = new string(readMode);
    players = new std::vector<Player*>{};
    gameMap = new Map("gameMap");
    deck = new Deck(20);

    if (*commandReadMode == "-console") {
        cout << "Taking commands from console!" << endl;
        commandProcessor = new CommandProcessor();
        log->AddSubject(*commandProcessor);
    } else {
        cout << "Taking commands from file!" << endl;
        commandProcessor = new FileCommandProcessorAdapter(
                readMode.substr(readMode.find(' ') + 1, readMode.size() + 1));
        log->AddSubject(*commandProcessor);
    }

    // Creating the vectors. One vector for each state where a vector contains
    // all the possible actions from each state.
    auto *start = new vector<string>();
    start->emplace_back("loadmap");
    start->emplace_back("tournament");

    auto *tournament = new vector<string>();
    tournament->emplace_back("start");
    tournament->emplace_back("gameStart");

    auto *mapLoaded = new vector<string>();
    mapLoaded->emplace_back("loadmap");
    mapLoaded->emplace_back("validatemap");

    auto *mapValidated = new vector<string>();
    mapValidated->emplace_back("addplayer");

    auto *playersAdded = new vector<string>();
    playersAdded->emplace_back("addplayer");
    playersAdded->emplace_back("gamestart");

    auto *assignReinforcement = new vector<string>();
    assignReinforcement->emplace_back("issueorder");

    auto *issueOrders = new vector<string>();
    issueOrders->emplace_back("issueorder");
    issueOrders->emplace_back("issueordersend");

    auto *executeOrders = new vector<string>();
    executeOrders->emplace_back("execorder");
    executeOrders->emplace_back("endexecorders");
    executeOrders->emplace_back("win");

    auto *winVector = new vector<string>();
    winVector->emplace_back("replay");
    winVector->emplace_back("quit");
    winVector->emplace_back("tournament");

    // Linking states to their vector of possible actions.
    stateMap->insert(make_pair("start", start));
    stateMap->insert(make_pair("tournament", tournament));
    stateMap->insert(make_pair("maploaded", mapLoaded));
    stateMap->insert(make_pair("mapvalidated", mapValidated));
    stateMap->insert(make_pair("playersadded", playersAdded));
    stateMap->insert(make_pair("assignreinforcement", assignReinforcement));
    stateMap->insert(make_pair("issueorders", issueOrders));
    stateMap->insert(make_pair("executeorders", executeOrders));
    stateMap->insert(make_pair("win", winVector));

    // Linking actions to their corresponding description to trigger the action and the effect of that action.
    descriptionMap->insert(make_pair("loadmap", make_tuple("Load a file with your map(s): loadmap <mapfile>", 1,
                                                           "Loading map from file with the following name:")));
    descriptionMap->insert(make_pair("tournament", make_tuple("Configure a tournament with the given maps, number of players/strategies, number of games per map and maximum number of turns: tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>", 8,
                           "Creating a tournament with configuration:")));
    descriptionMap->insert(make_pair("validatemap", make_tuple("Validate the given file with map(s): validatemap", 0,
                                                               "Validating the map.")));
    descriptionMap->insert(make_pair("addplayer", make_tuple("Add a new player to the game: addplayer <playername>", 2,
                                                             "Adding a player with the following name and strategy:")));
    descriptionMap->insert(make_pair("gamestart", make_tuple(
            "Automatically assign each country to a player and start the game: gamestart", 0,
            "Assigning the countries and starting the game.")));
    descriptionMap->insert(make_pair("issueorder", make_tuple("Issue an order: issueorder <ordernumber>", 1,
                                                              "Issuing the following order:")));
    descriptionMap->insert(
            make_pair("issueordersend", make_tuple("End the phase of issuing orders: issueordersend", 0,
                                                   "Ending the phase to issue orders.")));
    descriptionMap->insert(make_pair("execorder", make_tuple("Execute an order: execorder <ordernumber>", 1,
                                                             "Executing the following order:")));
    descriptionMap->insert(
            make_pair("endexecorders", make_tuple("End the phase of executing orders: endexecorders", 0,
                                                  "Ending the phase to execute orders.")));
    descriptionMap->insert(
            make_pair("win", make_tuple("Win the game (temporary option): win", 0, "Winning the game.")));
    descriptionMap->insert(
            make_pair("replay", make_tuple("Play another game: replay", 0, "Triggering the start of another game.")));
    descriptionMap->insert(make_pair("quit", make_tuple("Quit the game: quit", 0,
                                                        "Signaling the end of the game. Exiting the program...")));

    // Creating pointers to functions that will be called for the corresponding action.
    Game_Engine_Mem_Fn loadMap = &GameEngine::loadMap;
    Game_Engine_Mem_Fn tournamentMethod = &GameEngine::tournament;
    Game_Engine_Mem_Fn validateMap = &GameEngine::validateMap;
    Game_Engine_Mem_Fn addPlayer = &GameEngine::addPlayer;
    Game_Engine_Mem_Fn gameStart = &GameEngine::gameStart;
    Game_Engine_Mem_Fn issueOrder = &GameEngine::issueOrder;
    Game_Engine_Mem_Fn issueOrdersEnd = &GameEngine::issueOrdersEnd;
    Game_Engine_Mem_Fn execOrder = &GameEngine::execOrder;
    Game_Engine_Mem_Fn endExecOrders = &GameEngine::endExecOrders;
    Game_Engine_Mem_Fn winMethod = &GameEngine::win;
    Game_Engine_Mem_Fn replayMethod = &GameEngine::replay;
    Game_Engine_Mem_Fn quitMethod = &GameEngine::quit;

    // Linking the trigger keyword of an action to their method handler and the transition state.
    functionMap->insert(make_pair("loadmap", make_pair(loadMap, "maploaded")));
    functionMap->insert(make_pair("tournament", make_pair(tournamentMethod, "assignreinforcement")));
    functionMap->insert(make_pair("validatemap", make_pair(validateMap, "mapvalidated")));
    functionMap->insert(make_pair("addplayer", make_pair(addPlayer, "playersadded")));
    functionMap->insert(make_pair("gamestart", make_pair(gameStart, "assignreinforcement")));
    functionMap->insert(make_pair("issueorder", make_pair(issueOrder, "issueorders")));
    functionMap->insert(make_pair("issueordersend", make_pair(issueOrdersEnd, "executeorders")));
    functionMap->insert(make_pair("execorder", make_pair(execOrder, "executeorders")));
    functionMap->insert(make_pair("endexecorders", make_pair(endExecOrders, "assignreinforcement")));
    functionMap->insert(make_pair("win", make_pair(winMethod, "win")));
    functionMap->insert(make_pair("replay", make_pair(replayMethod, "start")));
    functionMap->insert(make_pair("quit", make_pair(quitMethod, "end")));
}

// Copy constructor.
GameEngine::GameEngine(const GameEngine &e) {
    this->currentState = new string(*e.currentState);
    this->stateMap = new map(*e.stateMap);
    this->descriptionMap = new map(*e.descriptionMap);
    this->functionMap = new map(*e.functionMap);
}

// Swaps the member data between two GameEngine objects.
void GameEngine::swap(GameEngine &first, GameEngine &second) {
    std::swap(first.currentState, second.currentState);
    std::swap(first.stateMap, second.stateMap);
    std::swap(first.descriptionMap, second.descriptionMap);
    std::swap(first.functionMap, second.functionMap);
}

// Destructor.
GameEngine::~GameEngine() {
    delete currentState;
    delete stateMap;
    delete descriptionMap;
    delete functionMap;
    delete commandProcessor;
    delete commandReadMode;
}

// The way this method works is that it first creates a local temporary copy of the given object (called ge)
// and method calls the swap function on the caller object (which is a GameEngine obj that was created
// with the default constructor) and on the locally created object. The swap method will effectively swap
// the member data between those two objects and so the caller object will now have the properties of the
// given object.
GameEngine &GameEngine::operator=(GameEngine ge) {
    swap(*this, ge);
    return *this;
}

// Defining the output operator.
std::ostream &operator<<(ostream &stream, const GameEngine &ge) {
    return stream << "GameEngine information:" << "\n Command Read Mode: " << *ge.commandReadMode
                  << "\n Current state: " << *ge.getCurrentState() << "\n Command Processor info:\n"
                  << *ge.commandProcessor;
}

// Getter for the currentState.
string *GameEngine::getCurrentState() const {
    return currentState;
}

// Setter for the currentState.
void GameEngine::transition(const string &state) {
    this->currentState = new string(state);
    Notify(*this);
}

// Getter for the stateMap.
std::map<string, vector<string> *> *GameEngine::getStateMap() const {
    return stateMap;
}

// Setter for the stateMap.
void GameEngine::setStateMap(const std::map<string, vector<string> *> &mapOfStates) {
    this->stateMap = new std::map(mapOfStates);
}

// Getter for the descriptionMap.
std::map<string, tuple<string, int, string>> *GameEngine::getDescriptionMap() const {
    return descriptionMap;
}

// Setter for the descriptionMap.
void GameEngine::setDescriptionMap(const std::map<string, tuple<string, int, string>> &mapOfDescriptions) {
    this->descriptionMap = new std::map(mapOfDescriptions);
}

// Getter for the functionMap.
std::map<string, pair<GameEngine::Game_Engine_Mem_Fn, string>> *GameEngine::getFunctionMap() const {
    return functionMap;
}

// Setter for the functionMap.
void GameEngine::setFunctionMap(const std::map<string, pair<GameEngine::Game_Engine_Mem_Fn, string>> &mapOfFunctions) {
    this->functionMap = new std::map(mapOfFunctions);
}

// Getter for the commandProcessor
CommandProcessor *GameEngine::getCommandProcessor() const {
    return commandProcessor;
}

// Setter for the commandProcessor
void GameEngine::setCommandProcessor(const CommandProcessor &cmdProcessor) {
    this->commandProcessor = new CommandProcessor(cmdProcessor);
}

// Getter for the commandReadMode.
string *GameEngine::getCommandReadMode() const {
    return commandReadMode;
}

// Setter for the commandReadMode.
void GameEngine::setCommandReadMode(const string &cmdReadMode) {
    this->commandReadMode = new string(cmdReadMode);
}

// Getter for the players.
std::vector<Player*>* GameEngine::getPlayers() const {
    return players;
}

// Setter for the players.
void GameEngine::setPlayers(std::vector<Player *> &newPlayers) {
    delete this->players;
    this->players = &newPlayers;
}

// Getter for the Map.
Map *GameEngine::getMap() const {
    return gameMap;
}

// Setter for the Map.
void GameEngine::setMap(const string &filename) {
    delete this->gameMap;
    this->gameMap = MapLoader::load(filename);
}

//// TODO Change the description of the start method once A2 is finished.
//// Contains the main while loop of the game which creates the game flow. Based on the current state,
//// a list of actions are fetched from the stateMap, then for each action fetched, a description and
//// trigger keyword are fetched from the description map. Then finally, based on the trigger keyword
//// entered by the user, the corresponding handler method is fetched from the function map which will
//// be executed in order to execute the action wanted by the user.
//void GameEngine::start() {
//    // <0> A boolean indicating if the command is valid or not
//    // <1> A string corresponding to the command keyword without arguments
//    // <2> A string saying that the command is valid or why the command is invalid
//    tuple<bool, string, string> optionInfo;
//    Command *command;
//
//    cout << "\n********************************************\n" << endl;
//    cout << "*************Welcome to Warzone!************" << endl;
//    cout << "\n********************************************\n" << endl;
//
//    while (*currentState != "end") {
//        printActionsIfNeeded();
//
//        //////////////////
//
//        //TODO Change logic here so that commands are retrieved automatically only in two cases: non-stop, but only until
//        // 1. From the start state until the arriving in the assignreinforcement state
//        // 2. From the win state until either exiting OR until arriving in the assignreinforcement state again
//        command = commandProcessor->getCommand(*this, *this->log);
//        if (command == nullptr) {
//            cout << "\nReached end of the file. Exiting..." << endl;
//            break;
//        }
//
//        /////////////////
//
//        optionInfo = commandProcessor->validate(*this, *command);
//
//        if (get<0>(optionInfo)) {
//            cout << "\nExecuting valid command!" << endl;
//            command->saveEffect(get<2>(descriptionMap->at(get<1>(optionInfo))), true);
//            std::invoke(functionMap->at(get<1>(optionInfo)).first, this, functionMap->at(get<1>(optionInfo)).second,
//                        *command->getCommandArgs());
//        } else {
//            cout << "\nCommand is not valid and will not be executed!" << endl;
//            command->saveEffect(get<2>(optionInfo), false);
//        }
//
//        cout << "\nRe-printing current command:\n" << *command << endl;
//    }
//}

// A function that prints the actions available for the user if setting up the game from the console.
void GameEngine::printActionsIfNeeded() {
    if (*commandReadMode != "-console") {
        return;
    }

    cout << "\n****************************************\n" << endl;
    cout << "\nHere are the current actions you can take:" << endl;

    for (const string &action: *stateMap->at(*currentState)) {
        cout << "\n- " << get<0>(descriptionMap->at(action)) << endl;
    }
}

// A function which will load the game map using the map class.
// Currently just changes the current state of the game to mapLoaded.
void GameEngine::loadMap(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the load map function! You are loading a map from the file: " << *commandArgs.at(1) << endl;
    cout << "\nThis is the state before the action: " << *currentState << endl;

    string filename = *commandArgs.at(1);

    // Check that file exists
    ifstream file;
    file.open(MAPS_DIR + filename);
    if (!file) {
        // File doesn't exist
        cerr << "File doesn't exist. Please enter another file" << endl;
    } else {
        // File exists, read the map
        try {
            setMap(filename);
            transition(transitionState);
        } catch (std::runtime_error &exp) {
            // Catch all exceptions defined as runtime errors
            cerr << exp.what() << endl;
        }
    }

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will attempt to create a tournament
void GameEngine::tournament(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the tournament function! You are creating a tournament!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    // The commandArgs array must follow this format:
    //  commandArgs[0] == "tournament" // already validated by command processor
    //  commandArgs[1] == "-M"
    //  commandArgs[2] == {list of map files, delimited by commas}
    //  commandArgs[3] == "-P"
    //  commandArgs[4] == {list of player strategies, delimited by commas}
    //  commandArgs[5] == "-G"
    //  commandArgs[6] == {number of games per map played}
    //  commandArgs[7] == "-D"
    //  commandArgs[8] == {maximum number of turns}

    // Validate command order
    const string VALID_PARAM_NAMES[] = {"-M", "-P", "-G", "-D"};

    bool isValidTournament = true;

    for (int i = 0; i < 4; i++) {
        if (*commandArgs[i*2+1] != VALID_PARAM_NAMES[i]) {
            cout << "Error parsing arguments: expected arg " << (i*2+1) << " as \"" << VALID_PARAM_NAMES[i] << "\", instead received \"" << *commandArgs[1] << "\"" << endl;
            isValidTournament = false;
        }
    }

    // Create vectors of maps and players
    parseTournamentMaps(*commandArgs[2]);
    parseTournamentPlayers(*commandArgs[4]);

    // Validate number of games per map
    int gamesPerMap = -1;
    try {
        gamesPerMap = stoi(*commandArgs[6]);
    } catch (exception &e) {
        cout << "Error: Cannot parse -G argument as integer -- " << e.what() << endl;
    }

    if (gamesPerMap < 1 || gamesPerMap > 5) {
        cout << "Error: Provided invalid games per map. Number must be between 1 and 5" << endl;
        isValidTournament = false;
    }

    // Validate maximum number of turns
    int maxTurns = -1;
    try {
        maxTurns = stoi(*commandArgs[8]);
    } catch (exception &e) {
        cout << "Error: Cannot parse -D argument as integer -- " << e.what() << endl;
    }

    if (maxTurns < 10 || maxTurns > 50) {
        cout << "Error: Provided invalid max number of turns. Number must be between 10 and 50" << endl;
        isValidTournament = false;
    }

    if (isValidTournament) {
        transition(transitionState);
    }

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

bool GameEngine::parseTournamentPlayers(const string &playersLine) const {
    bool isValidPlayers = false;
    const string VALID_STRATEGIES[] = {"aggressive", "benevolent", "neutral", "cheater"};

    vector<string> playerStrategies;
    extractCsv(&playersLine, playerStrategies);

    // Validate number of player strategies
    if (playerStrategies.size() < 2 || playerStrategies.size() > 4) {
        cout << "Error: Provided " << playerStrategies.size() << " players. Number must be between 2 and 4" << endl;
        isValidPlayers = false;
    }

    // Validate all player strategies
    for (const auto &playerStrategy : playerStrategies) {
        bool foundMatch = false;

        // Check that the current player strategy is a valid name for a strategy
        for (const auto &validStrategy : VALID_STRATEGIES) {
            if (playerStrategy == validStrategy) {
                foundMatch = true;
                break;
            }
        }

        if (!foundMatch) {
            cout << "Error: stategy name \"" << playerStrategy << "\" is not a valid strategy. "
                    << R"(Possible values are "aggressive", "benevolent", "neutral", "cheater")" << endl;
        }
    }

    return isValidPlayers;
}

bool GameEngine::parseTournamentMaps(const string &mapsLine) const {
    bool isValidMapsLine = false;

    vector<string> mapFiles;
    extractCsv(&mapsLine, mapFiles);
    vector<Map*> tournamentMaps;

    if (mapFiles.empty() || mapFiles.size() > 5) {
        cout << "Error: Provided " << mapFiles.size() << " map files. Number must be between 1 and 5" << endl;
        isValidMapsLine = false;
    } else {
        // Load and validate each map file

        for (const auto &mapFile : mapFiles) {
            // Check that file exists
            ifstream file;
            file.open(MAPS_DIR + mapFile);
            if (!file) {
                // File doesn't exist
                cout << "Error: File " << mapFile << " doesn't exist" << endl;
                isValidMapsLine = false;
            } else {
                // File exists, create the map
                try {
                    // Load map from file
                    Map *loadedMap = MapLoader::load(mapFile);

                    // Validate loaded map
                    loadedMap->validate();

                    // If map is valid, add to the list of tournament maps
                    tournamentMaps.emplace_back(MapLoader::load(mapFile));
                } catch (runtime_error &exp) {
                    // Catch all exceptions defined as runtime errors
                    cerr << "Error: Could not load/validate map " << mapFile << endl;
                    cerr << exp.what() << endl;
                }
            }
        }
    }

    return isValidMapsLine;
}

void GameEngine::extractCsv(const string *csvLine, vector<string> &csvVector) const {
    stringstream stream = stringstream(*csvLine);
    string token;
    while (getline(stream, token, ',')) {
        csvVector.emplace_back(token);
    }
}

// A function which will validate the game map using the map class.
// Currently just changes the current state of the game to mapValidated.
void GameEngine::validateMap(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the validate map function! You are validating a map!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    try {
        gameMap->validate();
        cout << "Map is valid!" << endl;

        transition(transitionState);
    } catch (std::runtime_error &exp) {
        // Catch all exceptions defined as runtime errors
        cerr << "Error: Map is not valid!" << endl;
        cerr << exp.what() << endl;
    }

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will add a player to the game using the player class.
// Currently just changes the current state of the game to playersAdded.
void GameEngine::addPlayer(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the add a player function! You are adding a player with name: " << *commandArgs.at(1) << " with the strategy: " << *commandArgs.at(2) << endl;
    cout << "\nThis is the state before the action: " << *currentState << endl;

    // Note: only a maximum of 6 players are supported
    if (players->size() >= 6) {
        cerr << "Error: Cannot add more than 6 players to game" << endl;
    } else {
        bool playerExists = false;
        // Check if player already exists
        for (const auto &player : *players) {
            if (*player->getPName() == *commandArgs.at(1)) {
                cerr << "Error: Player with name " << *commandArgs.at(1) << " already exists" << endl;
                playerExists = true;
                break;
            }
        }

        if (!playerExists) {
            cout << "Adding player " << *commandArgs.at(1) << " with " << *commandArgs.at(2) << " strategy to the list of players" << endl;


            Player *player = new Player(*commandArgs.at(1), &getStrategyObjectByStrategyName(*commandArgs.at(2)));
            players->emplace_back(player);
            this->log->AddSubject(*player->getOrdersList());

            transition(transitionState);
        }
    }

    cout << "\nCurrent list of players:" << endl;
    for (const auto &player : *players) {
        cout << "\t" << *player->getPName() << " - " << player->getPlayerStrategy()->printStrategy() << endl;
    }
    cout << endl;

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will assign each country to a player at the start of the game using the map class.
// Currently just changes the current state of the game to assignReinforcement.
void GameEngine::gameStart(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the game start function! You are assigning countries and will start the game!" << endl;
    cout << "\nThis is the state before the action: " << *currentState << endl;

    if (players->size() < 2) {
        cerr << "Not enough players to start the game!" << endl;
    } else {
        // Enough players to begin the game!
        // Shuffle list of players to randomize order of play
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        default_random_engine randomEngine(seed);
        shuffle(players->begin(), players->end(), randomEngine);

        cout << "\nOrder of play:" << endl;
        for (const auto &player : *players) {
            cout << "\t" << *player->getPName() << endl;
        }
        cout << endl;

        cout << "\nAssigning territories to each player..." << endl;
        // Distribute all territories to every player in a fair manner:
        // Iterate through a random permutation of the players vector
        // When the iterator reaches the end of the vector, generate a new permutation and move iterator back to the start
        vector<Player *> unpickedPlayers;
        auto iterator = unpickedPlayers.cbegin();

        for (int i = 0; i < gameMap->getContinentsSize(); i++) {
            Continent *continent = gameMap->getContinentByID(i+1);

            for (const auto &territory : continent->getTerritories()) {
                if (iterator == unpickedPlayers.cend()) {
                    unpickedPlayers = *players;
                    randomEngine.seed(std::chrono::system_clock::now().time_since_epoch().count());
                    shuffle(unpickedPlayers.begin(), unpickedPlayers.end(), randomEngine);
                    iterator = unpickedPlayers.cbegin();
                }

                // Iterator points to the next player to be assigned a territory
                if(!*static_cast<Player*>(*iterator)->getIsNeutral()){
                    static_cast<Player*>(*iterator)->acquireTerritory(territory);
                }
                cout << "Territory " << territory->getId() << " (" << territory->getName() << ") is owned by player " << *static_cast<Player>(**iterator).getPName() << endl;

                // Point iterator to next player
                iterator++;
            }
        }

        cout << "\nGiving 50 armies and 2 cards to each player" << endl;

        // Give 50 initial armies to each player
        for (auto player : *players) {
            player->increasePool(50);
            cout << "Player " << *player->getPName() << " has army count " << *player->getReinforcementPool() << endl;

            // Draw 2 cards per player
            for (int i = 0; i < 2; ++i) {
                // Only draw if there are cards left in the deck
                if (!deck->getWarzoneCards()->empty()) {
                    deck->draw(*player->getHand());
                }
            }

            cout << "Player " << *player->getPName() << " has drawn cards in their hand" << endl;
            cout << *player->getHand() << endl;
        }

        // Initialize the turn count to 1
        turnCount = 1;

        transition(transitionState);
    }
    cout << "\nThis is the state after the action: " << *currentState << endl;
    cout << "\nGame is about to start!" << endl;
}

// A function which will allow the issuing of an order using the orders_list class.
// Currently just changes the current state of the game to issueOrders.
void GameEngine::issueOrder(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the issue order function! You are issuing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will quit the order issuing phase using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void GameEngine::issueOrdersEnd(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the quit issue orders function! You are ending the order issuing phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will allow the execution of an order using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void GameEngine::execOrder(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the execute order function! You are executing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will quit the order execution phase using the orders_list class.
// Currently just changes the current state of the game to assignReinforcement.
void GameEngine::endExecOrders(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the quit execute orders function! You are ending the order execution phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    // Increment the turn count
    turnCount++;
    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will signal the quit of the current game once a player controls all the countries.
// Currently just changes the current state of the game to win.
void GameEngine::win(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the win function! You won!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start a new game after the current game has ended.
// Currently just changes the current state of the game to start.
void GameEngine::replay(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the replay function! You are starting a new game!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;

    transition(transitionState);

    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start the shut-down process of the game once the current game has ended.
// Currently just changes the current state of the game to quit.
void GameEngine::quit(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    transition(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;

    cout << "\nThank you for playing Risk! Shutting down game..." << endl;
}

PlayerStrategy& GameEngine::getStrategyObjectByStrategyName(string& name) {
    PlayerStrategy* ps;

    if(name == "human")
        ps = new HumanPlayerStrategy();
    else if(name == "aggressive")
        ps = new AggressivePlayerStrategy();
    else if(name == "benevolent")
        ps = new BenevolentPlayerStrategy();
    else if(name == "neutral")
        ps = new NeutralPlayerStrategy();
    else if(name == "cheater")
        ps = new CheaterPlayerStrategy();

    return *ps;
}

// Free function in order to test the functionality of the GameEngine for assignment #1.
void game_engine_driver(const string &cmdArg) {

    cout << "Took-in the following command argument: " << cmdArg << endl;

    GameEngine gameEngine(cmdArg);

    cout << "\nRunning game engine driver!" << endl;


    cout << "\n********************************************\n" << endl;
    cout << "*************Welcome to Warzone!************" << endl;
    cout << "\n********************************************\n" << endl;

    gameEngine.start();
}

void GameEngine::start() {
    while (*currentState != "end") {
        // Initial state at startup

        setPlayers(*(new vector<Player *>{}));

        deck->setDeck(*(new Deck(20)));

        transition("start");

        if(startupPhase()){
            break;
        }

        mainGameLoop();

        vector<string> states = vector{string("end"), string("start")};

        if(readingCommands(states)){
            break;
        }
    }
}


void GameEngine::mainGameLoop() {
    bool gameOver = false;
    while (!gameOver) {

        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        cout << "Player array size " << players->size() << " | Game over status " << gameOver << endl;

        //Check if a player owns at least 1 territory, remove this player if the player does not own any territory
        for (auto i = 0; i < players->size(); i++) {
            if (players->at(i)->getTerritories()->empty()) {
                cout << "Player " << *players->at(i)->getPName()
                     << " has no territories left. Player is therefore eliminated." << endl;
                delete players->at(i);
                players->erase(players->begin() + i);

                cout << "Waiting for input before continuing (enter 0)..." << endl;
                int val;
                cin >> val;
            }
        }
        gameOver = checkForWin();
        if(!gameOver){
            for(Player* player: *players){
                player->clearPlayerFriends();
                if (player->hasConqueredTerritoryInTurn()) {
                    deck->draw(*player->getHand());
                    player->setConqueredTerritoryInTurn(false);
                }
            }
        }
    }

    transition("win");

    cout << "Waiting for input before continuing (enter 0)..." << endl;
    int val;
    cin >> val;

//    cout << "Waiting 10 seconds before continuing..." << endl;
//    std::this_thread::sleep_for(std::chrono::seconds (10));
}

void GameEngine::reinforcementPhase(){

    cout << "\n*****************************Assigning Reinforcement Phase ...*****************************\n" << endl;

    for(auto & player : *players){

        //Give player an amount of armies depending on their number of territories, minimum that can be given is 3
        int currentNumberToGive = max(3, static_cast<int>(floor(static_cast<float>(player->getTerritories()->size())/3.0)));

        //Give to player an amount of armies depending of the continents they own (owning a continent means having all territories on that continent)
        for(auto i = 1; i <= gameMap->getContinentsSize(); i++){

            if(gameMap->getContinentByID(i)->isCompletelyOwned(*player)){
                currentNumberToGive += gameMap->getContinentByID(i)->getArmyBonusNumber();
            }

        }

        cout << "\nplayer->getTerritories()->size(): " << player->getTerritories()->size() << endl;
        cout << "Giving " << currentNumberToGive << " armies to " << *player->getPName() << endl;

        //Increasing the reinforcement pool of the player with the amounts calculated above
        player->increasePool(currentNumberToGive);

        cout << "Current number of armies: " << *player->getReinforcementPool() << endl;
    }
}

void GameEngine::issueOrdersPhase(){
    cout << "\n*****************************Starting Issuing Phase ...*****************************\n" << endl;
    vector<int> multipliers = {1000,100,10,9,8,7,6,5,4,3,2};
    for(auto & player : *players){ //for each player
            cout << "issueOrdersPhase() for player " << *player->getPName() << endl;
            //Issue orders related to defend the player's territories
            vector<tuple<Territory*,Territory*,string>> territoriesToDefend = player->toDefend();
//            for (tuple<Territory*,Territory*,string> tuple: territoriesToDefend) {
//                cout << "Source territory: " << get<0>(tuple) << " | Target territory: " << get<1>(tuple) << " | Order type: " << get<2>(tuple) << endl;
//            }
            int numOfDeploys = 0;
            for(auto& territoryTuple: territoriesToDefend){
                if(get<2>(territoryTuple) == "deploy"){
                    numOfDeploys++;
                }
            }
            int currentDeployIndex = 0;
            cout << "issuing orders for defend" << endl;
            for(auto& territoryTuple: territoriesToDefend){
                if(get<2>(territoryTuple) == "airlift"){
                    Order* airlift = new Airlift(*player,*get<0>(territoryTuple),*get<1>(territoryTuple), get<0>(territoryTuple)->getNumberOfArmies()/3);
                    player->getHand()->getHandsCards()->at(player->hasCard(3))->play(*deck, *player, airlift);
                    cout << "**issueOrder Airlift | Player: " << *player->getPName() << " | Source territory: " << get<0>(territoryTuple)->getName() << " | Target territory: " << get<1>(territoryTuple)->getName() << " | Armies: " << get<0>(territoryTuple)->getNumberOfArmies()/3 << endl;
                    this->log->AddSubject(*airlift);
                }
                else if(get<2>(territoryTuple) == "deploy"){
                    int reinforcementPool = *player->getReinforcementPool();
                    currentDeployIndex++;
                    Order* deploy = new Deploy(*player, *get<0>(territoryTuple), reinforcementPool/numOfDeploys);
                    cout << "**issueOrder Deploy | Player: " << *player->getPName() << " | Target territory: " << get<0>(territoryTuple)->getName() << " | Armies: " << reinforcementPool/numOfDeploys << endl;
                    player->issueOrder(deploy);
                    this->log->AddSubject(*deploy);
                    player->decreasePool(reinforcementPool/numOfDeploys);
                }
                else if(get<2>(territoryTuple) == "negotiate"){
                    cout << "issueOrder Negotiate" << endl;
                    Order* negotiate = new Negotiate(*player,*get<1>(territoryTuple)->getOwner());
                    player->getHand()->getHandsCards()->at(player->hasCard(4))->play(*deck, *player, negotiate);
                    cout << "**issueOrder Negotiate | Player: " << *player->getPName() << " | Enemy player: " << *get<1>(territoryTuple)->getOwner() << endl;
                    this->log->AddSubject(*negotiate);
                }
                else if(get<2>(territoryTuple) == "blockade"){
                    if(this->getNeutralPlayer() == nullptr){
                        players->emplace_back(new Player("Neutral", new NeutralPlayerStrategy()));
                        cout << "Created new Neutral player" << endl;
                    }

                    Order* blockade = new Blockade(*player,*this->getNeutralPlayer(),*get<1>(territoryTuple));
                    player->getHand()->getHandsCards()->at(player->hasCard(2))->play(*deck, *player, blockade);
                    cout << "**issueOrder Blockade | Player: " << *player->getPName() << " | Neutral player: " << *this->getNeutralPlayer()->getPName() << " | Target territory: " << get<1>(territoryTuple)->getName() << endl;
                    this->log->AddSubject(*blockade);
                }
                else if(get<2>(territoryTuple) == "advance"){
                    Order* advance = new Advance(*player,*get<0>(territoryTuple),*get<1>(territoryTuple),get<0>(territoryTuple)->getNumberOfArmies()/3);
                    player->issueOrder(advance);
                    cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(territoryTuple)->getName() << " | Target territory: " << get<1>(territoryTuple)->getName() << " | Armies: "<< get<0>(territoryTuple)->getNumberOfArmies()/3 << endl;
                    this->log->AddSubject(*advance);
                }
            }
            //Issue the orders related to attack other territories
            cout << "issuing orders for attack" << endl;
            for(auto& territoryTuple: player->toAttack()){
                if(get<2>(territoryTuple) == "advance"){
                    cout << "issueOrder advance" << endl;
                    Order* advance = nullptr;
                    for(int multiplier:multipliers){
                        cout << "Comparing " << get<0>(territoryTuple)->getNumberOfArmies() << " armies from " <<
                        get<0>(territoryTuple)->getName() << " vs " << get<1>(territoryTuple)->getNumberOfArmies() <<
                        " armies from " << get<1>(territoryTuple)->getName() << endl;
                        if(get<0>(territoryTuple)->getNumberOfArmies() > multiplier * (get<1>(territoryTuple)->getNumberOfArmies()) && (get<1>(territoryTuple)->getNumberOfArmies()) > 0){
                            cout << "Sending "<<multiplier<<"x the armies from " << get<0>(territoryTuple)->getName() << " to " << get<1>(territoryTuple)->getName() << endl;
                            cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(territoryTuple)->getName() << " | Target territory: " << get<1>(territoryTuple)->getName() << " | Armies: "<< get<1>(territoryTuple)->getNumberOfArmies()*multiplier << endl;
                            advance = new Advance(*player,*get<0>(territoryTuple),*get<1>(territoryTuple),get<1>(territoryTuple)->getNumberOfArmies()*multiplier);
                            break;
                        }
                    }
                    if(advance == nullptr){
                        cout << "doing +1 number of armies" << endl;
                        advance = new Advance(*player,*get<0>(territoryTuple),*get<1>(territoryTuple),get<1>(territoryTuple)->getNumberOfArmies()+1);
                        cout << "**issueOrder Advance | Player: " << *player->getPName() << " | Source territory: " << get<0>(territoryTuple)->getName() << " | Target territory: " << get<1>(territoryTuple)->getName() << " | Armies: "<< get<1>(territoryTuple)->getNumberOfArmies()+1 << endl;
                    }
                    player->issueOrder(advance);

                    this->log->AddSubject(*advance);
                }
                else if(get<2>(territoryTuple) == "bomb"){
                    cout << "issueOrder bomb" << endl;
                    Order* bomb = new Bomb(*player,*get<1>(territoryTuple));
                    player->getHand()->getHandsCards()->at(player->hasCard(0))->play(*deck, *player, bomb);
                    cout << "**issueOrder Bomb | Player: " << *player->getPName() << " | Target territory: " << get<1>(territoryTuple)->getName() << endl;
                    this->log->AddSubject(*bomb);
                }
            }
    }
}

//For each player, executes the deploy orders first from their respective orderlist and then the other orders
void GameEngine::executeOrdersPhase(){
    cout << "\n*****************************Executing Orders Phase ...*****************************\n" << endl;
    cout << "\n%%% Map Before Execution %%%" << endl;
    cout << gameMap << endl;
    for(Player* player : *players){
            //execute deploy orders first
            cout << "executeOrdersPhase --> Player: " << *player->getPName() << " | NumOrders: " << player->getOrdersList()->getOrders()->size() << endl;
            int loopIndex = 0;
            while(loopIndex < player->getOrdersList()->getOrders()->size()){
                if (player->getOrdersList()->getOrders()->at(loopIndex)->getDescription()->find("deploy") != std::string::npos) {
                    player->getOrdersList()->getOrders()->at(loopIndex)->execute();
                    delete player->getOrdersList()->getOrders()->at(loopIndex);
                    player->getOrdersList()->getOrders()->erase(player->getOrdersList()->getOrders()->begin()+loopIndex);
                }
                else {
                    loopIndex++;
                }
            }
    }
    //execute the other orders after in a round-robin fashion
    int loopIndex = 0;
    int maxOrderToExecute = 0;
    for(Player* player: *players){
        int sizeOfOrdersList = player->getOrdersList()->getOrders()->size();
        if(sizeOfOrdersList > maxOrderToExecute){
            maxOrderToExecute = sizeOfOrdersList;
        }
    }
    //Number of round-robins to do
    for(int i = 0; i < maxOrderToExecute; i++){
        for(Player* player: *players){
            //Checks if the player has any order left in their orders list
            if(player->getOrdersList()->getOrders()->size() != 0){
                player->getOrdersList()->getOrders()->at(loopIndex)->execute();
                delete player->getOrdersList()->getOrders()->at(loopIndex);
                player->getOrdersList()->getOrders()->erase(player->getOrdersList()->getOrders()->begin());
            }
        }
    }
    cout << "\n%%% Map After Execution %%%" << endl;
    cout << gameMap << endl;
}

//Checks if the player owns all the territories of the game map
bool GameEngine::checkForWin(){
    for(auto i = 0; i < players->size(); i++){
        if(players->at(i)->getTerritories()->size() == gameMap->getSize()){
            cout << "Player "<< *players->at(i)->getPName() << " has captured all territories and won!" << endl;
            return true;
        }
    }
    return false;
}

Player* GameEngine::getNeutralPlayer(){
    for(Player* player: *players){
        if (*player->getIsNeutral()) {
            return player;
        }
    }
    return nullptr;
}

bool GameEngine::startupPhase() {
    cout << "\nStartup phase\n" << endl;

    vector<string> states = vector{string("assignreinforcement")};

    return readingCommands(states);
}

bool GameEngine::readingCommands(const vector<string> &states) {
    printActionsIfNeeded();
    Command *nextCommand = commandProcessor->getCommand(*this, *log);;
    tuple<bool, string, string> commandProcessorResult;

    while (true) {
        if (nextCommand == nullptr) {
            cerr << "\nReached end of the file. Exiting..." << endl;
            return true;
        }

        // Validate against current state
        commandProcessorResult = commandProcessor->validate(*this, *nextCommand);

        if (get<0>(commandProcessorResult)) {
            // Command is valid, execute action given the state

            nextCommand->saveEffect(get<2>(descriptionMap->at(get<1>(commandProcessorResult))), true);

            std::invoke(
                    functionMap->at(get<1>(commandProcessorResult)).first, // Command name - as a string
                    this, // Object being called
                    functionMap->at(get<1>(commandProcessorResult)).second, // Pair of function reference and transition state
                    *nextCommand->getCommandArgs() // Arguments
            );

        } else {
            // Report bad input
            // TODO: Introduce better reporting
            nextCommand->saveEffect(get<2>(commandProcessorResult), false);
            cerr << "BAD INPUT! " << get<2>(commandProcessorResult) << endl;
            cerr << "Remaining in state " << *getCurrentState() << endl;
        }

        for (const string &state: states) {
            if (*currentState == state) {
                cout << "Waiting for input before continuing (enter 0)..." << endl;
                int val;
                cin >> val;
                return false;
            }
        }

        printActionsIfNeeded();

        // Get next command from command processor
        nextCommand = commandProcessor->getCommand(*this, *log);
    }
}

string GameEngine::stringToLog() const {
    std::string message =  std::string("The state of the game engine has been changed, it is now at: ") + std::string(*currentState);
    return message;
}
