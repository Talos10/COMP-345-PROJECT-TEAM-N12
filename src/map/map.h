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

    list<Territory> territories;
public:
    const list<Territory> &getTerritories() const;

public:
    Continent(string name, string color, int armyBonusNumber);
    int getArmyBonusNumber() const;
    bool isCompletelyOwned() const;

    bool isEmpty();

    void addTerritory(const Territory& territory);

public:
    const string &getName() const;

private:
    bool isSubgraph() const;
};

class Territory {
    private:
       const int index;
       const string name;
       const int x;
       const int y;
       const Continent continent;
        list<Territory> neighbours;
public:
    Territory(const int index, const string &name, const int x, const int y, const Continent &continent);

private:
    int numberOfArmies;
       // TODO: Change to Player type
       string owner;

public:
    const string &getName() const;
    const list<Territory> &getNeighbours() const;
    const Continent &getContinent() const;
    int getNumberOfArmies() const;
    const string &getOwner() const;
//    public:
//    void

    void addNeighbour(const Territory& territory);
};

class Map {
private:
    // Adjacency list of graph implemented using Vector
    vector<Territory> territories;
    vector<Continent> continents;
    const string name;
public:
    // Graph functions go here
    void validate();

    void addTerritory(const Territory& territory);

    Territory getTerritoryByID(int i);

    void addEdge(int originID, int destID);

    unsigned int getSize();

    string toMermaid();

    void addContinent(const Continent& continent);

    Continent &getContinentByID(int continentID);

    unsigned int getContinentsSize();
};

class MapLoader {

public:
    static Map load(const string& filename);
};

#endif //COMP_345_PROJECT_TEAM_N12_MAP_H
