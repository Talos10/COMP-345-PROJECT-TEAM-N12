#ifndef COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H
#define COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "command_processor/command_processing.h"
#include "player/player.h"
#include "map/map.h"
#include "observer/logging_observer.h"
#include "game_log/log_observer.h"

using namespace std;

// This class implements the flow of the game (i.e. the different phases of the game like
// reinforcement deployment or order execution) so that it can be played.

class GameEngine : public Subject, public ILoggable {
// Defining a type of pointer to any void and two params (the transition state and the arguments of the command to be executed).
    typedef void (GameEngine::*Game_Engine_Mem_Fn)(const string &transitionState, const vector<string *> &commandArgs);

private:
    // An observer
    LogObserver* log;

    // A string which holds the current state in which the game is (e.g. players added or issue orders).
    string *currentState;

    // A map of states to a vector of possible actions that can be taken from that state.
    map<string, vector<string> *> *stateMap;

    // A map of action keywords to a tuple of the action description, the amount of arguments needed for the action, and
    // the description of the effect of the action.
    map<string, tuple<string, int, string>> *descriptionMap;

    // A map of action trigger keywords to the corresponding method that will handle the action and the transition state
    // the game will be in after the action is executed.
    map<string, pair<Game_Engine_Mem_Fn, string>> *functionMap;

    // An object used to process the commands given to set up the game.
    CommandProcessor *commandProcessor;

    // A string which indicates whether the commands to start the game will be taken in via the console or from a file.
    string *commandReadMode;

    //A collection of players present in the game.
    std::vector<Player *> *players;

    //The map the players will fight on.
    Map *gameMap;

    //The deck from which the players will draw cards
    Deck *deck;

    //The neutral player of the game (if they exist yet)
    static Player *neutralPlayer;

    // The turn count of the current game
    int turnCount;

    // Defining the output operator for the GameEngine object.
    friend std::ostream &operator<<(std::ostream &stream, const GameEngine &ge);

    // A function used in the assignment operator definition which swaps the member data
    // between two GameEngine objects.
    void swap(GameEngine &first, GameEngine &second);

    // A function which will load the game map using the map class. It takes in the transition state the game will be in
    // after the method is executed and the arguments of the command that triggered the execution of this method.
    void loadMap(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will set up a new tournament. It takes in the transition state the game will be in
    // after the method is executed and the arguments of the command that triggered the execution of this method.
    void tournament(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will validate the game map using the map class. It takes in the transition state the game will
    // be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void validateMap(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will add a player to the game using the player class. It takes in the transition state the game
    // will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void addPlayer(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will assign each country to a player using the map class and will then start the game. It takes
    // in the transition state the game will be in after the method is executed and the arguments of the command that
    // triggered the execution of this method.
    void gameStart(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will allow the issuing of an order using the orders_list class. It takes in the transition state
    // the game will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void issueOrder(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will quit the order issuing phase using the orders_list class. It takes in the transition state
    // the game will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void issueOrdersEnd(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will allow the execution of an order using the orders_list class. It takes in the transition state
    // the game will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void execOrder(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will quit the order execution phase using the orders_list class. It takes in the transition state
    // the game will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void endExecOrders(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will signal the quit of the current game once a player controls all the countries. It takes in
    // the transition state the game will be in after the method is executed and the arguments of the command that
    // triggered the execution of this method.
    void win(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will start a new game after the current game has ended. It takes in the transition state the game
    // will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void replay(const string &transitionState, const vector<string *> &commandArgs);

    // A function which will start the shut-down process of the game once the current game has ended. It takes in the
    // transition state the game will be in after the method is executed and the arguments of the command that triggered the execution of this method.
    void quit(const string &transitionState, const vector<string *> &commandArgs);

    // A function which sets the player's strategy
    PlayerStrategy &getStrategyObjectByStrategyName(string &name);

public:

    // One param constructor which initializes all the maps, the current game state, and the mode in which the game
    // start-up commands will be read (from console or file).
    GameEngine(const string &commandReadMode);

    // A copy constructor.
    GameEngine(const GameEngine &ge);

    // Defining the assignment operator.
    GameEngine &operator=(GameEngine ge);

    // Destructor.
    ~GameEngine();

    // Getter for currentState.
    [[nodiscard]] string *getCurrentState() const;

    // Setter for currentState
    void transition(const string &state);

    // Getter for stateMap.
    [[nodiscard]] map<string, vector<string> *> *getStateMap() const;

    // Setter for mapOfStates.
    void setStateMap(const map<string, vector<string> *> &mapOfStates);

    // Getter for descriptionMap.
    [[nodiscard]] map<string, tuple<string, int, string>> *getDescriptionMap() const;

    // Setter for descriptionMap.
    void setDescriptionMap(const map<string, tuple<string, int, string>> &descriptionMap);

    // Getter for functionMap.
    [[nodiscard]] map<string, pair<GameEngine::Game_Engine_Mem_Fn, string>> *getFunctionMap() const;

    // Setter for functionMap.
    void setFunctionMap(const map<string, pair<GameEngine::Game_Engine_Mem_Fn, string>> &functionMap);

    // Getter for the commandProcessor.
    [[nodiscard]] CommandProcessor *getCommandProcessor() const;

    // Setter for the commandProcessor.
    void setCommandProcessor(const CommandProcessor &commandProcessor);

    // Getter for the commandReadMode.
    [[nodiscard]] string *getCommandReadMode() const;

    // Setter for the commandReadMode.
    void setCommandReadMode(const string &state);

    // Getter for the players.
    [[nodiscard]] std::vector<Player*>* getPlayers() const;

    // Setter for the players.
    void setPlayers(std::vector<Player *> &newPlayers);

    // Getter for the Map.
    [[nodiscard]] Map *getMap() const;

    // Setter for the Map.
    void setMap(const string &filename);

    // A function which allows the user to start a game of Risk.
    void start();

    // A function to execute the startup phase of the game
    bool startupPhase();

    // A function that prints the actions available for the user if setting up the game from the console.
    void printActionsIfNeeded();

    //Function that will run the main game loop for the gameplay part
    void mainGameLoop();

    //Function that will take care of the Reinforcement Phase part of the main game loop
    void reinforcementPhase();

    //Function that will take care of the Issue Orders part of the main game loop
    void issueOrdersPhase();

    //Function that will take care of the Orders Execution Phase part of the main game loop
    void executeOrdersPhase();

    //Function that will check if there is a player that has won the game
    bool checkForWin();

    // A function which sets the neutral player
    static void setNeutralPlayer(Player *ntrPlayer);

    // A function which returns the neutral player (if they exist)
    static Player *getNeutralPlayer();

    // Override class from ILoggable
    string stringToLog() const override;

    bool readingCommands(const vector<string> &states);

    // TODO: Document each function here
    void extractCsv(const string *csvLine, vector<string> &csvVector) const;

    bool parseTournamentMaps(const string &mapsLine) const;

    bool parseTournamentPlayers(const string &playersLine) const;
};

// Free function in order to test the functionality of the GameEngine for assignment #2. Takes in a commandline argument
// which specifies if the commands are to be read through the console (-console) or from a file (-file <filename>).
void game_engine_driver(const string &cmdArg);



#endif //COMP_345_PROJECT_TEAM_N12_GAME_ENGINE_H
