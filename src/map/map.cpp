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
    for (Continent continent : continents) {
        if (continent.isEmpty()) {
            cerr << "Continent " << continent.getName() << " does not have a country" << endl;
            throw;
        }
    }
//TODO validate() Definition
}

void Map::addTerritory(const Territory& territory) {
    territories.emplace_back(territory);
}

Territory Map::getTerritoryByID(int i) {
    return territories[i - 1];
}

void Map::addEdge(int originID, int destID) {
    territories[originID - 1].addNeighbour(territories[destID - 1]);
}

unsigned int Map::getSize() {
    return territories.size();
}

string Map::toMermaid() {
    /*
     * flowchart TB
    c1-->a2
    subgraph one
    a1-->a2
    end
    subgraph two
    b1-->b2
    end
    subgraph three
    c1-->c2
    end
     */
    string mermaid = "flowchart TB\n";
    for (const auto &continent : continents) {
        cout << "--debug:" << continent.getName() << endl;
        mermaid.append("subgraph " + continent.getName() + " -- " + std::to_string(continent.getArmyBonusNumber()) + "\n");
        for (const auto &territory : continent.getTerritories()) {
            cout << "--debug:" << territory.getName() << endl;
            for (const auto &neighbour : territory.getNeighbours()) {
                cout << "--debug:" << territory.getName() << "--" << neighbour.getName() << endl;
                mermaid.append(territory.getName() + "-->" + neighbour.getName() + "\n");
            }
        }
        mermaid.append("end\n");
    }
    return mermaid;
}

void Map::addContinent(const Continent& continent) {
    continents.emplace_back(continent);

}

Continent &Map::getContinentByID(int continentID) {
    return continents[continentID - 1];
}

unsigned int Map::getContinentsSize() {
    return continents.size();
}

Continent::Continent(string name, string color, int armyBonusNumber) :                          name(std::move(name)),
                                                                                                color(std::move(color)),
                                                                                                armyBonusNumber(
                                                                                                        armyBonusNumber) {}

int Continent::getArmyBonusNumber() const {
    return armyBonusNumber;
}

bool Continent::isCompletelyOwned() const {
    if (territories.empty()) return false;

    // TODO: Use Player Class
    string owner;
    for (const auto &territory : territories) {
        const string& nextOwner = territory.getOwner();
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

const string &Continent::getName() const {
    return name;
}

bool Continent::isEmpty() {
    return territories.empty();
}

void Continent::addTerritory(const Territory& territory) {
    territories.emplace_back(territory);
}

const list<Territory> &Continent::getTerritories() const {
    return territories;
}

const string &Territory::getName() const {
    return name;
}

const list<Territory> &Territory::getNeighbours() const {
    return neighbours;
}

const Continent &Territory::getContinent() const {
    return continent;
}

int Territory::getNumberOfArmies() const {
    return numberOfArmies;
}

const string &Territory::getOwner() const {
    return owner;
}

Territory::Territory(const int index, const string &name, const int x, const int y, const Continent &continent) : index(
        index), name(name), x(x), y(y), continent(continent) {
    numberOfArmies = 0;

}

void Territory::addNeighbour(const Territory& territory) {
    neighbours.emplace_back(territory);
}

/**
 * Load a map from a given file
 * @param string the filename
 * @return the Map object
 */
Map MapLoader::load(const string& filename) {
    ifstream file(filename);
    string line;
    file.open(MAPS_DIR + filename);

    std::vector<std::string> sections;
    sections.emplace_back("files");
    sections.emplace_back("continents");
    sections.emplace_back("countries");
    sections.emplace_back("borders");
//    const string sections[] = {"files", "continents"};

    const std::regex sectionRegex(R"(^\[.*\]$)");
    const std::regex fileRegex(R"(^\S+ \S+$)", ECMAScript);
    const std::regex continentRegex(R"(^\S+ \d+ \S+$)", ECMAScript);
    const std::regex countryRegex(R"(^\d+ \S+ \d+ \d+ \d+$)", ECMAScript);
    const std::regex borderRegex(R"(^\d+( \d+)+$)", ECMAScript);

    Map gameMap;
    string currentSection = "head";
    int currentCountryIndex = 0;
    int currentBorderIndex = 0;

    while (std::getline(file, line)) {
        cout << line << endl;

        // ignore lines starting with ; or empty lines
        if (line.empty() || line.front() == ';') {
//            cout << "\tSkip!" << endl;
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

                        cout << "\tFound continent " << name << " of bonus value " << bonusNumber << " and colour " << color << endl;
                        Continent continent = Continent(name, color, bonusNumber);
                        gameMap.addContinent(continent);
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

                        // Validation step: Ensure country index increases sequentially from 1
                        if (currentCountryIndex + 1 != countryID) {
                            cerr << "\tExpected country index " << currentCountryIndex + 1 << ", got " << countryID << endl;
//                            throw;
                        } else {
                            currentCountryIndex++;
                        }

                        // Validation step: ensure continent exists
                        if (continentID <= 0 || continentID > gameMap.getContinentsSize()) {
                            cout << "\tTerritory has an invalid continent index: " << continentID << endl;
                        }

                        cout << "\tFound country " << countryName << " of continent index " << continentID << " and x,y of " << xCoordinate << "," << yCoordinate << endl;
                        Territory territory = Territory(countryID, countryName, xCoordinate, yCoordinate, gameMap.getContinentByID(continentID));

                        // Assign continent to territory
                        gameMap.getContinentByID(continentID).addTerritory(territory);

                        // Add territory to map
                        gameMap.addTerritory(territory);
                    } else {
                        cout << "\tCountry line doesn't meet expected format" << endl;
                    }
                    break;
                case BORDERS:
                    // TODO: Country list in countries/borders must be sequential according to index
                    // TODO: Borders must be bidirectional
                    if (std::regex_match(line, borderRegex)) {
                        // match -- parse country line
                        std::stringstream ss;
                        ss << line;

                        int originCountryID;
                        ss >> originCountryID;

                        // Validate -- first column must be sequential
                        if (currentBorderIndex + 1 != originCountryID) {
                            cerr << "\tExpected origin country index " << currentBorderIndex + 1 << ", got " << originCountryID << endl;
                            throw;
                        }

                        currentBorderIndex++;

                        int currentNeighbourID;
                        while (ss >> currentNeighbourID) {
                            // Validate -- neighbour must exist in map
                            if (currentNeighbourID <= 0 || currentNeighbourID > gameMap.getSize()) {
                                cerr << "\tNo neighbour in map with ID " << currentNeighbourID << endl;
                                throw;
                            } else {
                                gameMap.addEdge(originCountryID, currentNeighbourID);
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



    cout << "Closing file" << endl;
    file.close();

    cout << "Finished reading file into Map";
    return gameMap;
}
