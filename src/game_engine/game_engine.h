#ifndef COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H
#define COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

// This class implements the flow of the game (i.e. the different phases of the game like
// reinforcement deployment or order execution) so that it can be played.
class Game_Engine {
// Defining a type of pointer to any void and no args member function of this class.
    typedef void (Game_Engine::*Game_Engine_Mem_Fn)();

private:
    // A string which holds the current state in which the game is (e.g. players added or issue orders).
    string *currentState;

    // A map of states to a vector of possible actions that can be taken from that state.
    map<string, vector<string>> *stateMap;

    // A map of actions to a pair of the action description and the action trigger keyword.
    map<string, pair<string, string>> *descriptionMap;

    // A map of action trigger keywords to the corresponding method that will handle the action.
    map<string, Game_Engine_Mem_Fn> *functionMap;

    // Defining the output operator.
    friend std::ostream &operator<<(std::ostream &stream, const Game_Engine &ge);

    // A function which will load the game map using the map class.
    void loadMap();

    // A function which will validate the game map using the map class.
    void validateMap();

    // A function which will add a player to the game using the player class.
    void addPlayer();

    // A function which will assign each country to a player at the start of the game using the map class.
    void assignCountries();

    // A function which will allow the issuing of an order using the orders_list class.
    void issueOrder();

    // A function which will end the order issuing phase using the orders_list class.
    void endIssueOrders();

    // A function which will allow the execution of an order using the orders_list class.
    void execOrder();

    // A function which will end the order execution phase using the orders_list class.
    void endExecOrders();

    // A function which will signal the end of the current game once a player controls all the countries.
    void win();

    // A function which will start a new game after the current game has ended.
    void play();

    // A function which will start the shut-down process of the game once the current game has ended.
    void end();

public:
    // Default constructor which initializes all the maps and the current game state.
    Game_Engine();

    // A copy constructor.
    Game_Engine(const Game_Engine &ge);

    // Defining the assignment operator.
    Game_Engine &operator=(Game_Engine ge);

    // Destructor.
    ~Game_Engine();

    // Getter for currentState
    [[nodiscard]] string *getCurrentState() const;

    // Setter for currentState
    void setCurrentState(const string &state);

    // Getter for stateMap
    [[nodiscard]] map<string, vector<string>> *getStateMap() const;

    // Setter for mapOfStates
    void setStateMap(const map<string, vector<string>> &mapOfStates);

    // Getter for descriptionMap
    [[nodiscard]] map<string, pair<string, string>> *getDescriptionMap() const;

    // Setter for descriptionMap
    void setDescriptionMap(const map<string, pair<string, string>> &descriptionMap);

    // Getter for functionMap
    [[nodiscard]] map<string, Game_Engine_Mem_Fn> *getFunctionMap() const;

    // Setter for functionMap
    void setFunctionMap(const map<string, Game_Engine_Mem_Fn> &functionMap);

    // A function used in the assignment operator definition which swaps the member data
    // between two Game_Engine objects.
    void swap(Game_Engine &first, Game_Engine &second);

    // A function which allows the user to start a game of Risk.
    void start();
};

// Free function in order to test the functionality of the Game_Engine for assignment #1.
void game_engine_driver();

#endif //COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H
