#include "game_engine.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <math.h>

using namespace std;

// The implementation file of the GameEngine class.

// One param constructor which initializes all the maps, the current game state, and the mode in which the game
// start-up commands will be read (from console or file).
GameEngine::GameEngine(const string &readMode) {
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
    } else {
        cout << "Taking commands from file!" << endl;
        commandProcessor = new FileCommandProcessorAdapter(
                readMode.substr(readMode.find(' ') + 1, readMode.size() + 1));
    }

    // Creating the vectors. One vector for each state where a vector contains
    // all the possible actions from each state.
    auto *start = new vector<string>();
    start->emplace_back("loadmap");

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

    // Linking states to their vector of possible actions.
    stateMap->insert(make_pair("start", start));
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
    descriptionMap->insert(make_pair("validatemap", make_tuple("Validate the given file with map(s): validatemap", 0,
                                                               "Validating the map.")));
    descriptionMap->insert(make_pair("addplayer", make_tuple("Add a new player to the game: addplayer <playername>", 1,
                                                             "Adding a player with the following name:")));
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
void GameEngine::setCurrentState(const string &state) {
    this->currentState = new string(state);
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
void GameEngine::setPlayers(const std::vector<Player*> &players) {
    delete this->players;
    this->players = new std::vector(players);
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

// TODO Change the description of the start method once A2 is finished.
// Contains the main while loop of the game which creates the game flow. Based on the current state,
// a list of actions are fetched from the stateMap, then for each action fetched, a description and
// trigger keyword are fetched from the description map. Then finally, based on the trigger keyword
// entered by the user, the corresponding handler method is fetched from the function map which will
// be executed in order to execute the action wanted by the user.
void GameEngine::start() {
    // <0> A boolean indicating if the command is valid or not
    // <1> A string corresponding to the command keyword without arguments
    // <2> A string saying that the command is valid or why the command is invalid
    tuple<bool, string, string> optionInfo;
    Command *command;

    cout << "\n********************************************\n" << endl;
    cout << "*************Welcome to Warzone!************" << endl;
    cout << "\n********************************************\n" << endl;

    while (*currentState != "end") {
        printActionsIfNeeded();

        //////////////////

        //TODO Change logic here so that commands are retrieved automatically only in two cases: non-stop, but only until
        // 1. From the start state until the arriving in the assignreinforcement state
        // 2. From the win state until either exiting OR until arriving in the assignreinforcement state again
        command = commandProcessor->getCommand(*this);
        if (command == nullptr) {
            cout << "\nReached end of the file. Exiting..." << endl;
            break;
        }

        /////////////////

        optionInfo = commandProcessor->validate(*this, *command);

        if (get<0>(optionInfo)) {
            cout << "\nExecuting valid command!" << endl;
            command->saveEffect(get<2>(descriptionMap->at(get<1>(optionInfo))), true);
            std::invoke(functionMap->at(get<1>(optionInfo)).first, this, functionMap->at(get<1>(optionInfo)).second,
                        *command->getCommandArgs());
        } else {
            cout << "\nCommand is not valid and will not be executed!" << endl;
            command->saveEffect(get<2>(optionInfo), false);
        }

        cout << "\nRe-printing current command:\n" << *command << endl;
    }
}

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
    setCurrentState(transitionState);
    //setMap("canada-map.txt");
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will validate the game map using the map class.
// Currently just changes the current state of the game to mapValidated.
void GameEngine::validateMap(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the validate map function! You are validating a map!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will add a player to the game using the player class.
// Currently just changes the current state of the game to playersAdded.
void GameEngine::addPlayer(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the add a player function! You are adding a player with name: " << *commandArgs.at(1) << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will assign each country to a player at the start of the game using the map class.
// Currently just changes the current state of the game to assignReinforcement.
void GameEngine::gameStart(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the game start function! You are assigning countries and will start the game!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;

    mainGameLoop();
}

// A function which will allow the issuing of an order using the orders_list class.
// Currently just changes the current state of the game to issueOrders.
void GameEngine::issueOrder(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the issue order function! You are issuing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will quit the order issuing phase using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void GameEngine::issueOrdersEnd(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the quit issue orders function! You are ending the order issuing phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will allow the execution of an order using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void GameEngine::execOrder(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the execute order function! You are executing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will quit the order execution phase using the orders_list class.
// Currently just changes the current state of the game to assignReinforcement.
void GameEngine::endExecOrders(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the quit execute orders function! You are ending the order execution phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will signal the quit of the current game once a player controls all the countries.
// Currently just changes the current state of the game to win.
void GameEngine::win(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the win function! You won!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start a new game after the current game has ended.
// Currently just changes the current state of the game to start.
void GameEngine::replay(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Inside the replay function! You are starting a new game!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start the shut-down process of the game once the current game has ended.
// Currently just changes the current state of the game to quit.
void GameEngine::quit(const string &transitionState, const vector<string *> &commandArgs) {
    cout << "\n****************************************\n" << endl;
    cout << "Thank you for playing Risk! Shutting down game..." << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(transitionState);
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// Free function in order to test the functionality of the GameEngine for assignment #1.
void game_engine_driver(const string &cmdArg) {

    cout << "Took-in the following command argument: " << cmdArg << endl;

    GameEngine gameEngine(cmdArg);

    cout << "\nRunning game engine driver!" << endl;



    gameEngine.start();
}

void GameEngine::mainGameLoop(){
    //TODO remove this part once Donya finishes part 2
    gameMap = MapLoader::load("canada-map.txt");
    bool gameOver = false;
    players->emplace_back(new Player("obama"));
    players->emplace_back(new Player("talos"));
    cout << "\nAssigning an arbitrary territory to the players:\n" << endl;
    players->at(0)->acquireTerritory(gameMap->getTerritoryByID(1)); //Continent 1 Territory 1
    players->at(1)->acquireTerritory(gameMap->getTerritoryByID(8));//Continent 3 Territory 12
    //cout << gameMap->getTerritoryByID(1) << " with a numArmies of " << gameMap->getTerritoryByID(1)->getNumberOfArmies() << endl;
    //end TODO


    //Check if a player owns at least 1 territory, remove this player if the player does not own any territory
    for(auto i = 0; i < players->size(); i++){
        if(players->at(i)->getTerritories()->empty()){
            cout << "Player "<< *players->at(i)->getPName() << " has no territories left. Player is therefore eliminated." << endl;
            players->erase(players->begin() + i);
        }
    }

    while(!gameOver){

        reinforcementPhase();

        //This big for-loop will take the different decision considering the neighbors of a player's
        //territories
        for(auto & player : *players){ //for each player

            for(auto & territory : *player->getTerritories()){ //for each territory of a player

                for(auto i = 0; i < territory->getNeighbours().size(); i++) { //for each neighbor of a territory

                    //players->at(i)->issueOrder()
                    auto neighbor = territory->getNeighbours().begin();
                    std::advance(neighbor,i);
                    if(static_cast<Territory*>(*neighbor)->getOwner() == nullptr)
                        cout << *neighbor << " is the neighbor of " << territory <<
                        " and has no owner " << endl;
                    else
                        cout << *neighbor << " is the neighbor of " << territory <<
                        " and has owner " << *static_cast<Territory*>(*neighbor)->getOwner()->getPName() << endl;

                }
            }

        }

        executeOrdersPhase();

        cout << "Deck: " << *deck << endl;

        gameOver = checkForWin();

        break;

    }

}

void GameEngine::reinforcementPhase(){

    cout << "\nAssigning Reinforcement Phase ...\n" << endl;

    for(auto & player : *players){

        for(auto j = 0; j < player->getTerritories()->size(); j++){

            player->getTerritories()->at(j)->addNumberArmy(static_cast<int>(floor(static_cast<float>(player->getTerritories()->size())/3.0)));

        }
    }

}

void GameEngine::executeOrdersPhase(){

    cout << "\nExecuting Orders Phase ...\n" << endl;

    for(auto & player : *players){

        //TODO execute the orders from the ordersList for each player

    }

}

//Checks if the player owns all the territories of the game map
bool GameEngine:: checkForWin(){
    for(auto i = 0; i < players->size(); i++){
        if(players->at(i)->getTerritories()->size() == gameMap->getSize()){
            cout << "Player "<< *players->at(i)->getPName() << " has captured all territories and won!" << endl;
            return true;
        }
    }
    return false;
}
