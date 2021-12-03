// Describe the use of the map class.

#ifndef COMP_345_PROJECT_TEAM_N12_MAP_H
#define COMP_345_PROJECT_TEAM_N12_MAP_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "player/player.h"

#define MAPS_DIR "../maps/"

using std::list;
using std::string;
using std::vector;

class Territory;
class Player;

class Continent{
private:
    string name;
    string colour;
    int armyBonusNumber;
    vector<Territory*> territories;
public:
    Continent(const string &name, const string &color, int armyBonusNumber);

    const string &getName() const;
    int getArmyBonusNumber() const;
    const vector<Territory *> &getTerritories() const;

    const string &getColour() const;

    bool isCompletelyOwned(Player& player) const;
    bool isEmpty() const;
    void addTerritory(Territory* territory);

    // Copy constructor
    Continent(const Continent& continent);

    // Stream insertion operator
    friend std::ostream & operator << (std::ostream &out, const Continent* continent);

    // Assignment operator
    Continent& operator=(Continent other);
};

class Territory {
private:
    int id;
    string name;
    int x;
    int y;
    int continentId;
//    const Continent* continent;
    list<Territory*> neighbours;
    int numberOfArmies;
    Player* owner;
public:
    Territory(int id, const string &name, int x, int y, int continentId);
    const string &getName() const;
    int getContinentId() const;
    int getId() const;
    list<Territory *> &getNeighbours();
    int getNumberOfArmies() const;
    Player* getOwner() const;
    void setOwner(Player* player);
    void setNumberOfArmies(const int numArmies);
    void addNeighbour(Territory* territory);
    void addArmies(const int numArmies);
    void removeArmies(const int numArmies);

    int getX() const;

    int getY() const;

    void addNumberArmy(int numOfArmies);

    // Copy constructor
    Territory(const Territory& territory);

    // Stream insertion operator
    friend std::ostream & operator << (std::ostream &out, const Territory* territory);

    // Assignment operator
    Territory& operator=(Territory other);
};

class Map {
private:
    // Adjacency list of graph implemented using Vector
    vector<Territory*> territories;
    vector<Continent*> continents;
    string name;
    static bool isConnected(const vector<Territory*>& territories);
    static void dfs(Territory *const &currentTerritory, const vector<Territory *> &territories, list<int> &visitedTerritories);

public:
    // Graph functions go here
    void validate();
    void addTerritory(Territory* territory);
    Territory* getTerritoryByID(int i);
    void addEdge(int originID, int destID);

    unsigned int getSize();
    string getName();
    string toMermaid();
    void addContinent(Continent* continent);
    Continent* getContinentByID(int continentID);

    unsigned int getContinentsSize();

    Map(string name);

    // Copy constructor
    Map(const Map& map);

    // Stream insertion operator
    friend std::ostream &operator<<(std::ostream &out, Map* map);

    // Assignment operator
    Map& operator=(Map other);
};

class MapLoader {
public:
    static Map* load(const string& filename);
};

// Free function in order to test the functionality of the Map for assignment #1.
void map_driver(const string &filename);

#endif //COMP_345_PROJECT_TEAM_N12_MAP_H
