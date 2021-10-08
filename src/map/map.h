// Describe the use of the map class.

#ifndef COMP_345_PROJECT_TEAM_N12_MAP_H
#define COMP_345_PROJECT_TEAM_N12_MAP_H

#include <string>
#include <list>
#include <vector>
#include <iostream>

using std::list;
using std::string;
using std::vector;

class Territory;
class Continent{
private:
    const string name;
    const string colour;
    const int armyBonusNumber;
    vector<Territory*> territories;
public:
    Continent(const string &name, const string &color, int armyBonusNumber);

    const string &getName() const;
    int getArmyBonusNumber() const;
    const vector<Territory *> &getTerritories() const;

    const string &getColour() const;

    bool isCompletelyOwned() const;
    bool isEmpty() const;
    void addTerritory(Territory* territory);

    Continent(const Continent& continent);

    // Stream insertion operator
    friend std::ostream & operator << (std::ostream &out, const Continent* continent);

};

class Territory {
private:
    const int id;
    const string name;
    const int x;
    const int y;
    const Continent* continent;
    list<Territory*> neighbours;
    int numberOfArmies;
    // TODO: Change to Player type
    string owner;
public:
    Territory(int id, const string &name, int x, int y, const Continent *continent);
    const string &getName() const;
    const Continent *getContinent() const;
    int getId() const;
    list<Territory *> &getNeighbours();
    int getNumberOfArmies() const;
    const string &getOwner() const;
    void addNeighbour(Territory* territory);

    int getX() const;

    int getY() const;

    Territory(const Territory& territory);

    friend std::ostream & operator << (std::ostream &out, const Territory* territory);
};

class Map {
private:
    // Adjacency list of graph implemented using Vector
    vector<Territory*> territories;
    vector<Continent*> continents;
    const string name;
    static bool isConnected(const vector<Territory*>& territories);
    static void dfs(Territory *const &currentTerritory, const vector<Territory *> &territories, list<int> &visitedTerritories);

public:
    // Graph functions go here
    void validate();
    void addTerritory(Territory* territory);
    Territory* getTerritoryByID(int i);
    void addEdge(int originID, int destID);

    unsigned int getSize();
    string toMermaid();
    void addContinent(Continent* continent);
    Continent* getContinentByID(int continentID);

    unsigned int getContinentsSize();

    Map(string name);

    Map(const Map& map);
    friend std::ostream &operator<<(std::ostream &out, Map* map);
};

class MapLoader {
public:
    static Map* load(const string& filename);
};

#endif //COMP_345_PROJECT_TEAM_N12_MAP_H
