// Describe the use of the map class.

#ifndef COMP_345_PROJECT_TEAM_N12_MAP_H
#define COMP_345_PROJECT_TEAM_N12_MAP_H

#include <string>
#include <list>
#include <vector>

using std::list;
using std::string;
using std::vector;

class Territory;
class Continent{
private:
    const string name;
    const string color;
    const int armyBonusNumber;
    list<Territory*> territories;
    bool isSubgraph();
public:
    Continent(const string &name, const string &color, const int armyBonusNumber);

    const string &getName() const;
    const int getArmyBonusNumber() const;
    const list<Territory *> &getTerritories() const;

    bool isCompletelyOwned() const;
    bool isEmpty() const;
    void addTerritory(Territory* territory);
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
    Territory(const int id, const string &name, const int x, const int y, const Continent *continent);
    const string &getName() const;
    const Continent *getContinent() const;
    const int getId() const;
    list<Territory *> &getNeighbours();
    int getNumberOfArmies() const;
    const string &getOwner() const;
    void addNeighbour(Territory* territory);
};

class Map {
private:
    // Adjacency list of graph implemented using Vector
    vector<Territory*> territories;
    vector<Continent*> continents;
    const string name;
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
};

class MapLoader {
public:
    static Map* load(const string& filename);
};

#endif //COMP_345_PROJECT_TEAM_N12_MAP_H
