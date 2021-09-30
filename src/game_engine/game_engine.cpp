#include "game_engine.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>

using namespace std;

// The implementation file of the Game_Engine class.

// Default constructor which initializes all the maps and the current game state.
Game_Engine::Game_Engine() {
    // Map and state initializations
    currentState = new string("start");
    stateMap = new std::map<string, vector<string>>{};
    descriptionMap = new std::map<string, pair<string, string>>{};
    functionMap = new std::map<string, Game_Engine_Mem_Fn>{};

    // Creating the vectors. One vector for each state where a vector contains
    // all the possible actions from each state.
    vector<string> start;
    start.emplace_back("loadMap");

    vector<string> mapLoaded;
    mapLoaded.emplace_back("loadMap");
    mapLoaded.emplace_back("validateMap");

    vector<string> mapValidated;
    mapValidated.emplace_back("addPlayer");

    vector<string> playersAdded;
    playersAdded.emplace_back("addPlayer");
    playersAdded.emplace_back("assignCountries");

    vector<string> assignReinforcement;
    assignReinforcement.emplace_back("issueOrder");

    vector<string> issueOrders;
    issueOrders.emplace_back("issueOrder");
    issueOrders.emplace_back("endIssueOrders");

    vector<string> executeOrders;
    executeOrders.emplace_back("execOrder");
    executeOrders.emplace_back("endExecOrders");
    executeOrders.emplace_back("win");

    vector<string> winVector;
    winVector.emplace_back("play");
    winVector.emplace_back("end");

    // Linking states to their vector of possible actions.
    stateMap->insert(make_pair("start", start));
    stateMap->insert(make_pair("mapLoaded", mapLoaded));
    stateMap->insert(make_pair("mapValidated", mapValidated));
    stateMap->insert(make_pair("playersAdded", playersAdded));
    stateMap->insert(make_pair("assignReinforcement", assignReinforcement));
    stateMap->insert(make_pair("issueOrders", issueOrders));
    stateMap->insert(make_pair("executeOrders", executeOrders));
    stateMap->insert(make_pair("win", winVector));

    // Linking actions to their corresponding description and trigger keyword.
    descriptionMap->insert(make_pair("loadMap", make_pair("Load a file with your map(s).", "loadmap")));
    descriptionMap->insert(make_pair("validateMap", make_pair("Validate the given file with map(s).", "validatemap")));
    descriptionMap->insert(make_pair("addPlayer", make_pair("Add a new player to the game.", "addplayer")));
    descriptionMap->insert(make_pair("assignCountries", make_pair("Assign each country to a player and start the game!",
                                                                  "assigncountries")));
    descriptionMap->insert(make_pair("issueOrder", make_pair("Issue an order.", "issueorder")));
    descriptionMap->insert(
            make_pair("endIssueOrders", make_pair("End the phase of issuing orders.", "endissueorders")));
    descriptionMap->insert(make_pair("execOrder", make_pair("Execute an order.", "execorder")));
    descriptionMap->insert(
            make_pair("endExecOrders", make_pair("End the phase of executing orders.", "endexecorders")));
    descriptionMap->insert(make_pair("win", make_pair("Win the game (temporary option).", "win")));
    descriptionMap->insert(make_pair("play", make_pair("Play another game.", "play")));
    descriptionMap->insert(make_pair("end", make_pair("End the game.", "end")));

    // Creating pointers to functions that will be called for the corresponding action.
    Game_Engine_Mem_Fn loadMap = &Game_Engine::loadMap;
    Game_Engine_Mem_Fn validateMap = &Game_Engine::validateMap;
    Game_Engine_Mem_Fn addPlayer = &Game_Engine::addPlayer;
    Game_Engine_Mem_Fn assignCountries = &Game_Engine::assignCountries;
    Game_Engine_Mem_Fn issueOrder = &Game_Engine::issueOrder;
    Game_Engine_Mem_Fn endIssueOrders = &Game_Engine::endIssueOrders;
    Game_Engine_Mem_Fn execOrder = &Game_Engine::execOrder;
    Game_Engine_Mem_Fn endExecOrders = &Game_Engine::endExecOrders;
    Game_Engine_Mem_Fn winMethod = &Game_Engine::win;
    Game_Engine_Mem_Fn playMethod = &Game_Engine::play;
    Game_Engine_Mem_Fn endMethod = &Game_Engine::end;

    // Linking the trigger keyword of an action to their method handler.
    functionMap->insert(make_pair("loadmap", loadMap));
    functionMap->insert(make_pair("validatemap", validateMap));
    functionMap->insert(make_pair("addplayer", addPlayer));
    functionMap->insert(make_pair("assigncountries", assignCountries));
    functionMap->insert(make_pair("issueorder", issueOrder));
    functionMap->insert(make_pair("endissueorders", endIssueOrders));
    functionMap->insert(make_pair("execorder", execOrder));
    functionMap->insert(make_pair("endexecorders", endExecOrders));
    functionMap->insert(make_pair("win", winMethod));
    functionMap->insert(make_pair("play", playMethod));
    functionMap->insert(make_pair("end", endMethod));
}

// Copy constructor.
Game_Engine::Game_Engine(const Game_Engine &e) {
    this->currentState = new string(*e.currentState);
    this->stateMap = new map(*e.stateMap);
    this->descriptionMap = new map(*e.descriptionMap);
    this->functionMap = new map(*e.functionMap);
}

// Swaps the member data between two Game_Engine objects.
void Game_Engine::swap(Game_Engine &first, Game_Engine &second) {
    std::swap(first.currentState, second.currentState);
    std::swap(first.stateMap, second.stateMap);
    std::swap(first.descriptionMap, second.descriptionMap);
    std::swap(first.functionMap, second.functionMap);
}

// Destructor.
Game_Engine::~Game_Engine() {
    delete currentState;
    delete stateMap;
    delete descriptionMap;
    delete functionMap;
}

// The way this method works is that it first creates a local temporary copy of the given object (called ge)
// and method calls the swap function on the caller object (which is a Game_Engine obj that was created
// with the default constructor) and on the locally created object. The swap method will effectively swap
// the member data between those two objects and so the caller object will now have the properties of the
// given object.
Game_Engine &Game_Engine::operator=(Game_Engine ge) {
    swap(*this, ge);
    return *this;
}

// Defining the output operator.
std::ostream &operator<<(ostream &stream, const Game_Engine &ge) {
    return stream << "\nCurrent state: " << *ge.getCurrentState();
}

// Getter for the currentState.
string *Game_Engine::getCurrentState() const {
    return currentState;
}

// Setter for the currentState.
void Game_Engine::setCurrentState(const string &state) {
    this->currentState = new string(state);
}

// Getter for the stateMap.
map<string, vector<string>> *Game_Engine::getStateMap() const {
    return stateMap;
}

// Setter for the stateMap.
void Game_Engine::setStateMap(const map<string, vector<string>> &mapOfStates) {
    this->stateMap = new std::map(mapOfStates);
}

// Getter for the descriptionMap.
map<string, pair<string, string>> *Game_Engine::getDescriptionMap() const {
    return descriptionMap;
}

// Setter for the descriptionMap.
void Game_Engine::setDescriptionMap(const map<string, pair<string, string>> &mapOfDescriptions) {
    this->descriptionMap = new std::map(mapOfDescriptions);
}

// Getter for the functionMap.
map<string, Game_Engine::Game_Engine_Mem_Fn> *Game_Engine::getFunctionMap() const {
    return functionMap;
}

// Setter for the functionMap.
void Game_Engine::setFunctionMap(const map<string, Game_Engine::Game_Engine_Mem_Fn> &mapOfFunctions) {
    this->functionMap = new std::map(mapOfFunctions);
}

// Contains the main while loop of the game which creates the game flow. Based on the current state,
// a list of actions are fetched from the stateMap, then for each action fetched, a description and
// trigger keyword are fetched from the description map. Then finally, based on the trigger keyword
// entered by the user, the corresponding handler method is fetched from the function map which will
// be executed in order to execute the action wanted by the user.
void Game_Engine::start() {
    bool isOptionExist = false;
    vector<string> *actions;
    string x;

    cout << "\n****************************************\n" << endl;
    cout << "************Welcome to Risk!************" << endl;
    cout << "\n****************************************\n" << endl;

    while (*currentState != "end") {
        actions = &stateMap->at(*currentState);

        cout << "\n****************************************\n" << endl;
        cout << "\nHere are the current actions you can take:" << endl;

        for (const string &action: *actions) {
            cout << "\n- " << descriptionMap->at(action).first << " To select this option, enter the following word: "
                 << descriptionMap->at(action).second << endl;
        }

        do {
            cout << "\nPlease enter the word corresponding to the action you wish to take (without whitespaces):"
                 << endl;

            getline(cin, x);

            cout << "\nYou entered option: " << x << endl;

            try {
                std::invoke(functionMap->at(x), this);
                isOptionExist = true;
            } catch (out_of_range &) {
                cout << "\nThe entered option " << x << " does not exist. Please try again." << endl;
            }
        } while (!isOptionExist);
    }
}

// A function which will load the game map using the map class.
// Currently just changes the current state of the game to mapLoaded.
void Game_Engine::loadMap() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the load map function! You are loading a map!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("mapLoaded"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will validate the game map using the map class.
// Currently just changes the current state of the game to mapValidated.
void Game_Engine::validateMap() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the validate map function! You are validating a map!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("mapValidated"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will add a player to the game using the player class.
// Currently just changes the current state of the game to playersAdded.
void Game_Engine::addPlayer() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the add a player function! You are adding a player!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("playersAdded"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will assign each country to a player at the start of the game using the map class.
// Currently just changes the current state of the game to assignReinforcement.
void Game_Engine::assignCountries() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the assign countries function! You are assigning a country!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("assignReinforcement"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will allow the issuing of an order using the orders_list class.
// Currently just changes the current state of the game to issueOrders.
void Game_Engine::issueOrder() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the issue order function! You are issuing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("issueOrders"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will end the order issuing phase using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void Game_Engine::endIssueOrders() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the end issue orders function! You are ending the order issuing phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("executeOrders"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will allow the execution of an order using the orders_list class.
// Currently just changes the current state of the game to executeOrders.
void Game_Engine::execOrder() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the execute order function! You are executing an order!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("executeOrders"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will end the order execution phase using the orders_list class.
// Currently just changes the current state of the game to assignReinforcement.
void Game_Engine::endExecOrders() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the end execute orders function! You are ending the order execution phase!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("assignReinforcement"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will signal the end of the current game once a player controls all the countries.
// Currently just changes the current state of the game to win.
void Game_Engine::win() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the win function! You won!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("win"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start a new game after the current game has ended.
// Currently just changes the current state of the game to start.
void Game_Engine::play() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nInside the play function! You are starting a new game!" << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("start"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// A function which will start the shut-down process of the game once the current game has ended.
// Currently just changes the current state of the game to end.
void Game_Engine::end() {
    cout << "\n****************************************\n" << endl;
    cout << "\n\nThank you for playing Risk! Shutting down game..." << endl;

    cout << "\nThis is the state before the action: " << *currentState << endl;
    setCurrentState(string("end"));
    cout << "\nThis is the state after the action: " << *currentState << endl;
}

// Free function in order to test the functionality of the Game_Engine for assignment #1.
void game_engine_driver() {
    Game_Engine gameEngine;

    cout << "\nRunning game engine driver!" << endl;

    gameEngine.start();
}
