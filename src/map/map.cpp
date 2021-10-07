#include "map.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>
#include <string>

#define MAPS_DIR "../maps/"
using std::cout, std::endl, std::cerr;
using std::ifstream;
using std::find;
using std::regex_constants::ECMAScript;

enum section {HEAD, FILES, CONTINENTS, COUNTRIES, BORDERS, DEFAULT};
section getSectionFromString(const string& str) {
    if (str == "head") return HEAD;
    if (str == "files") return FILES;
    if (str == "continents") return CONTINENTS;
    if (str == "countries") return COUNTRIES;
    if (str == "borders") return BORDERS;
    return DEFAULT;
}


void Map::validate() {
    // Validation step: check no continents are empty
    cout << "Validating map..." << endl;
    for (const auto &continent : continents) {
        if (continent->isEmpty()) {
            cerr << "Continent " << continent->getName() << " does not have a country" << endl;
            throw;
        }
    }
    cout << "Success! No continents are empty!" << endl;

    // Validation step: continents have a mutually exclusive list of territories
    // Create array for territory existence
    vector<int> territoriesTaken(territories.size(), 0);
    for (const auto &continent : continents) {
        for (const auto &territory : continent->getTerritories()) {
            int &lookupValue = territoriesTaken.at(territory->getId() - 1);
            lookupValue++;
        }
    }

//    cout << "Territories Visited: [";
//    for (const auto &item : territoriesTaken) {
//        cout << " " << item << " ";
//    }
//    cout << "]" << endl;

    // All territories must be exactly visited once
    for (const auto &numberOfTimesVisited : territoriesTaken) {
        if (numberOfTimesVisited != 1) {
            cerr << "Territory must belong to one and only one continent, found " << numberOfTimesVisited;
            throw;
        }
    }
    cout << "Success! No continents share the same territory!" << endl;

    // Test Map is connected graph
    cout << "Checking for connectedness of whole map..." << endl;
    if (!isConnected(territories)) {
        cerr << "ERROR! Map is not connected!" << endl;
        throw;
    }
    cout << "Success! The global map is connected!" << endl;

    // Test each continent is connected subgraph of map
    for (const auto &continent : continents) {
        cout << "Checking for connectedness of continent "  << continent->getName() << "..." << endl;
        if (!isConnected(continent->getTerritories())) {
            cerr << "ERROR! Continent is not connected!" << endl;
            throw;
        } else {
            cout << "Success! Subgraph " << continent->getName() << " is valid!" << endl;

        }
    }

    cout << "Success! All subgraphs are valid!" << endl;
}

void Map::addTerritory(Territory* territory) {
    territories.emplace_back(territory);
}

Territory* Map::getTerritoryByID(int i) {
    return territories[i - 1];
}

/**
 * Adds an edge from one node in graph to another
 * @param originID the origin territory's ID value
 * @param destID the destination territory's ID value
 */
void Map::addEdge(int originID, int destID) {
    territories[originID - 1]->addNeighbour(territories[destID - 1]);
}

/**
 * Generates a representation of the Map in a Mermaid.js compliant flowchart.
 * The resulting text can be viewed at http://mermaid.live, or any editor that supports Mermaid.js
 * @return the string representation of the map
 */
string Map::toMermaid() {
    string mermaid = "\nflowchart TB\n";
    for (const auto &continent : continents) {
        mermaid.append("subgraph " + continent->getName() + ": " + std::to_string(continent->getArmyBonusNumber()) + "\n");
        for (const auto &territory : continent->getTerritories()) {
            mermaid.append(territory->getName() + "_" + std::to_string(territory->getId()) + "\n");
        }
        mermaid.append("end\n");
    }
    for (const auto &territory : territories) {
        for (const auto &neighbour : territory->getNeighbours()) {
            if (territory->getId() > neighbour->getId()) {
                continue;
            }
            mermaid.append(territory->getName() + "_" + std::to_string(territory->getId()) + "---" + neighbour->getName() + "_" + std::to_string(neighbour->getId()) + "\n");
        }
    }
    return mermaid;
}

unsigned int Map::getSize() {
    return territories.size();
}

void Map::addContinent(Continent *continent) {
    continents.emplace_back(continent);
}

Continent *Map::getContinentByID(int continentID) {
    return continents[continentID - 1];
}

unsigned int Map::getContinentsSize() {
    return continents.size();
}

Map::Map(string name) : name(name) {}

std::ostream &operator<<(std::ostream &out, Map* map) {
    out << "Map of name " << map->name;
    out << ", with countries :[";
    for (const auto &territory : map->territories) {
        out << territory << endl;
    }
    out << "] and continents: [" << endl;
    for (const auto &continent : map->continents) {
        out << continent << endl;
    }
    out << "]" << endl;

    return out;
}

bool Map::isConnected(const vector<Territory *>& territories) {
    // Build vector of territories visited
    list<int> visitedTerritoryIDs;

    // Do a DFS to visit all connected territories
    dfs(territories.front(), territories, visitedTerritoryIDs);

    // if all territories in the graph have been visited, then graph is connected
    for (const auto &item : territories) {
        auto isVisited = std::find(visitedTerritoryIDs.begin(), visitedTerritoryIDs.end(), item->getId());
        if (isVisited == std::end(visitedTerritoryIDs)) {
            cerr << "Territory " << item->getId() << " has not been visited!" << endl;
            return false;
        }
    }

    return true;
}

void Map::dfs(Territory *const &currentTerritory, const vector<Territory *> &territories, list<int> &visitedTerritories) {
    auto isAlreadyVisited = std::find(visitedTerritories.begin(), visitedTerritories.end(), currentTerritory->getId());
    if (isAlreadyVisited != std::end(visitedTerritories)) {
        // Already visited node, no point visiting its adjacent nodes
        return;
    }
    visitedTerritories.emplace_back(currentTerritory->getId());

    for (const auto &neighbour : currentTerritory->getNeighbours()) {
        auto isInTerritories = std::find(territories.begin(), territories.end(), neighbour);
        if (isInTerritories != std::end(territories)){
            dfs(neighbour, territories, visitedTerritories);
        }
    }
}

bool Continent::isCompletelyOwned() const {
    if (territories.empty()) return false;

    // TODO: Use Player Class
    string owner;
    for (const auto &territory : territories) {
        const string& nextOwner = territory->getOwner();
        if (owner.empty()) {
            if (nextOwner.empty()) {
                return false;
            } else {
                owner = nextOwner;
            }
        } else if (nextOwner != owner) {
            return false;
        }
    }
    return true;
}

bool Continent::isEmpty() const {
    return territories.empty();
}

void Continent::addTerritory(Territory* territory) {
    territories.emplace_back(territory);
}

Continent::Continent(const string &name, const string &color, const int armyBonusNumber) : name(name), colour(color),
                                                                                           armyBonusNumber(
                                                                                                   armyBonusNumber) {}

const string &Continent::getName() const {
    return name;
}

const int Continent::getArmyBonusNumber() const {
    return armyBonusNumber;
}

const vector<Territory *> &Continent::getTerritories() const {
    return territories;
}

std::ostream &operator<<(std::ostream &out, const Continent* continent) {
    out <<  "Continent " << continent->getName();
    out << " of bonus value " << continent->getArmyBonusNumber();
    out << " and colour " << continent->getColour();
    return out;
}

const string &Continent::getColour() const {
    return colour;
}

/**
 * Load a map from a given file
 * @param string the filename
 * @return the Map object
 */
Map* MapLoader::load(const string& filename) {
    ifstream file(filename);
    string line;
    file.open(MAPS_DIR + filename);

    std::vector<std::string> sections;
    sections.emplace_back("files");
    sections.emplace_back("continents");
    sections.emplace_back("countries");
    sections.emplace_back("borders");

    const std::regex sectionRegex(R"(^\[.*\]$)");
    const std::regex fileRegex(R"(^\S+ \S+$)", ECMAScript);
    const std::regex continentRegex(R"(^\S+ \d+ \S+$)", ECMAScript);
    const std::regex countryRegex(R"(^\d+ \S+ \d+ \d+ \d+$)", ECMAScript);
    const std::regex borderRegex(R"(^\d+( \d+)+$)", ECMAScript);

    Map* gameMap = new Map("myMap");

    string currentSection = "head";
    int currentCountryIndex = 0;
    int currentBorderIndex = 0;

    while (std::getline(file, line)) {
        cout << line << endl;

        // ignore lines starting with ; or empty lines
        if (line.empty() || line.front() == ';') {
            cout << "\tSkip!" << endl;
            continue;
        }

        // switch to new section if exists
        if (std::regex_match(line, sectionRegex)) {
            // TODO: Make sure sections are sequential
            string sectionName = line.substr(1, line.length()-2);
            if (find(sections.begin(), sections.end(), sectionName) != sections.end()) {
                cout << "\tFound new section: " << sectionName << "!" << endl;
                currentSection = sectionName;
            } else {
                cerr << "\tFound unknown section: " << sectionName << endl;
//                throw;
            }
        } else {
            // staying in current section
            switch (getSectionFromString(currentSection)) {
                case HEAD:
                    // Nothing should be in the head
                    cerr << "\tFound unknown line in head";
//                    throw;
                case FILES:
                    // parse file line as long as regex matches
                    if (std::regex_match(line, fileRegex)) {
                        // match -- parse file line
                        // ignore file line
                    } else {
                        cout << "\tFile line doesn't meet expected format" << endl;
                    }
                    break;
                case CONTINENTS:
                    if (std::regex_match(line, continentRegex)) {
                        // match -- parse continent line
                        std::stringstream ss;
                        ss << line;

                        string name;
                        int bonusNumber;
                        string color;
                        ss >> name >> bonusNumber >> color;

//                        cout << "\tFound continent " << name << " of bonus value " << bonusNumber << " and colour " << color << endl;
                        auto* continent = new Continent(name, color, bonusNumber);
                        cout << continent << endl;
                        gameMap->addContinent(continent);
                    } else {
                        cout << "\tContinent line doesn't meet expected format" << endl;
                    }
                    break;
                case COUNTRIES:
                    if (std::regex_match(line, countryRegex)) {
                        // match -- parse country line
                        std::stringstream ss;
                        ss << line;

                        int countryID;
                        string countryName;
                        int continentID;
                        int xCoordinate;
                        int yCoordinate;
                        string color;
                        ss >> countryID >> countryName >> continentID >> xCoordinate >> yCoordinate;

                        // Validation step: Ensure country id increases sequentially from 1
                        if (currentCountryIndex + 1 != countryID) {
                            cerr << "\tExpected country id " << currentCountryIndex + 1 << ", got " << countryID << endl;
//                            throw;
                        } else {
                            currentCountryIndex++;
                        }

                        // Validation step: ensure continent exists
                        if (continentID <= 0 || continentID > gameMap->getContinentsSize()) {
                            cout << "\tTerritory has an invalid continent id: " << continentID << endl;
                        }

//                        cout << "\tFound country " << countryName << " of continent id " << continentID << " and x,y of " << xCoordinate << "," << yCoordinate << endl;
                        auto* territory = new Territory(countryID, countryName, xCoordinate, yCoordinate, gameMap->getContinentByID(continentID));
                        cout << territory << endl;

                        // Assign continent to territory
                        gameMap->getContinentByID(continentID)->addTerritory(territory);

                        // Add territory to map
                        gameMap->addTerritory(territory);
                    } else {
                        cout << "\tCountry line doesn't meet expected format" << endl;
                    }
                    break;
                case BORDERS:
                    // TODO: Country list in countries/borders must be sequential according to id
                    // TODO: Borders must be bidirectional
                    if (std::regex_match(line, borderRegex)) {
                        // match -- parse country line
                        std::stringstream ss;
                        ss << line;

                        int originCountryID;
                        ss >> originCountryID;

                        // Validate -- first column must be sequential
                        if (currentBorderIndex + 1 != originCountryID) {
                            cerr << "\tExpected origin country id " << currentBorderIndex + 1 << ", got " << originCountryID << endl;
                            throw;
                        }

                        currentBorderIndex++;

                        int currentNeighbourID;
                        while (ss >> currentNeighbourID) {
                            // Validate -- neighbour must exist in map
                            if (currentNeighbourID <= 0 || currentNeighbourID > gameMap->getSize()) {
                                cerr << "\tNo neighbour in map with ID " << currentNeighbourID << endl;
                                throw;
                            } else {
                                gameMap->addEdge(originCountryID, currentNeighbourID);
                                cout << "\tAdded edge between countries " << originCountryID << " and " << currentNeighbourID << endl;
                            }
                        }
                    } else {
                        cout << "\tBorder line doesn't meet expected format" << endl;
                    }
                    break;
                case DEFAULT:
                default:
                    cout << "\tShouldn't be here!" << endl;
                    break;
            }
        }
    }

    cout << "Printout of Map:" << endl;
    cout << gameMap << endl;

    cout << "Closing file" << endl;
    file.close();

    cout << "Finished reading file into Map" << endl;
    return gameMap;
}

const string &Territory::getName() const {
    return name;
}

const Continent *Territory::getContinent() const {
    return continent;
}

list<Territory *> &Territory::getNeighbours() {
    return neighbours;
}

int Territory::getNumberOfArmies() const {
    return numberOfArmies;
}

const string &Territory::getOwner() const {
    return owner;
}

Territory::Territory(const int id, const string &name, const int x, const int y, const Continent *continent) : id(id),
                                                                                                               name(name),
                                                                                                               x(x),
                                                                                                               y(y),
                                                                                                               continent(
                                                                                                                       continent) {}

void Territory::addNeighbour(Territory *territory) {
    neighbours.emplace_back(territory);
}

const int Territory::getId() const {
    return id;
}

std::ostream &operator<<(std::ostream &out, const Territory *territory) {
    out << "Country " << territory->getId() << " of name " << territory->getName();
    out << " of continent " << territory->getContinent()->getName();
    out << " and x,y of " << territory->getX() << "," << territory->getY();
    return out;
}

const int Territory::getX() const {
    return x;
}

const int Territory::getY() const {
    return y;
}
