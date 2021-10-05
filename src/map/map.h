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
    int index;
    string name;
    string color;
    int armyBonusNumber;

    const list<Territory> territories;

public:
    Continent(int index, string name, string color, int armyBonusNumber);
    int getArmyBonusNumber() const;
    bool isCompletelyOwned() const;
};

class Territory {
    private:
       const int index;
       const string name;
       const int x;
       const int y;
       const list<Territory> neighbours;
       const Continent continent;

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

};

class Map {
private:
    // Adjacency list of graph implemented using Vector
    const vector<Territory> territories;
    const string name;
public:
    // Graph functions go here
    void validate();

};

class MapLoader {

public:
    Map load(const string& filename);
};

#endif //COMP_345_PROJECT_TEAM_N12_MAP_H
