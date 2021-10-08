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

//denotes possible values for section headers in map txt files
enum section {HEAD, FILES, CONTINENTS, COUNTRIES, BORDERS, DEFAULT};
section getSectionFromString(const string& str) {
    if (str == "head") return HEAD;
    if (str == "files") return FILES;
    if (str == "continents") return CONTINENTS;
    if (str == "countries") return COUNTRIES;
    if (str == "borders") return BORDERS;
    return DEFAULT;
}

///********************************///
/// Continent Class Implementation ///
///********************************///

/**
 * Constructs a Continent object
 * @param name The name of the continent
 * @param color The colour of the continent
 * @param armyBonusNumber The army bonus number (applies when the same player owns all countries in the same continent)
 */
Continent::Continent(const string &name, const string &color, const int armyBonusNumber) :
        name(name), colour(color), armyBonusNumber(armyBonusNumber) {}

/**
 * @return the name
 */
const string &Continent::getName() const {
    return name;
}

/**
 * @return the army bonus number
 */
int Continent::getArmyBonusNumber() const {
    return armyBonusNumber;
}

/**
 * @return A reference to the collection of territories
 */
const vector<Territory *> &Continent::getTerritories() const {
    return territories;
}

/**
 * @return the colour of the continent
 */
const string &Continent::getColour() const {
    return colour;
}

/**
 * Checks if all territories in the continent are owned by the same player
 * @return true if all territories in the continent are owned by the same player
 */
bool Continent::isCompletelyOwned() const {
    // Empty case: a continent with no territories has no owner
    if (territories.empty()) return false;

    // TODO: Use Player Class
    string owner;

    for (const auto &territory : territories) {
        const string& nextOwner = territory->getOwner();

        // In first iteration of loop, owner is not set
        if (owner.empty()) {
            // If the first owner is not set, continent is not completely owned
            if (nextOwner.empty()) {
                return false;
            } else {
                // Set the first owner as the owner to be compared against
                owner = nextOwner;
            }
        // If subsequent territories don't have the same owner as the first, continent is not fully owned
        } else if (nextOwner != owner) {
            return false;
        }
    }
    return true;
}

/**
 * @return true if the continent has no territories
 */
bool Continent::isEmpty() const {
    return territories.empty();
}

/**
 * Adds a territory to the continent's collection of territories
 * @param territory
 */
void Continent::addTerritory(Territory* territory) {
    // append territory to end of vector
    // IDE recommends emplace_back over push_back
    territories.emplace_back(territory);
}

/**
 * Copy constructor of the Continent class
 */
Continent::Continent(const Continent &continent) : name(continent.name), colour(continent.colour), armyBonusNumber(continent.armyBonusNumber) {
    // FIXME: Believe this is just a shallow copy
    this->territories = continent.territories;
}

/**
 * Stream insertion operator for Continent object
 * @param out the output stream
 * @param continent the continent
 * @return the output stream with a string representation of the Continent
 */
std::ostream &operator<<(std::ostream &out, const Continent* continent) {
    out <<  "Continent " << continent->getName();
    out << " of bonus value " << continent->getArmyBonusNumber();
    out << " and colour " << continent->getColour();
    return out;
}

///********************************///
/// Territory Class Implementation ///
///********************************///

/**
 * Constructs a Territory object as read from the map txt file
 * @param id the ID of the territory (first column)
 * @param name the name (second column)
 * @param x the x-coordinate (fourth column)
 * @param y the y-coordinate (fourth column)
 * @param continent a pointer to the continent belonging to the territory
 */
Territory::Territory(const int id, const string &name, const int x, const int y, const Continent *continent)
        : id(id), name(name), x(x), y(y), continent(continent) {
    numberOfArmies = 0;
}

/**
 * @return the name of the territory
 */
const string &Territory::getName() const {
    return name;
}

/**
 * @return a pointer to the continent of the territory
 */
const Continent *Territory::getContinent() const {
    return continent;
}

/**
 * @return the territory's ID (as seen in the map file)
 */
int Territory::getId() const {
    return id;
}

/**
 * @return a list of the Territories bordering the territory
 */
list<Territory *> &Territory::getNeighbours() {
    return neighbours;
}

/**
 * @return the current number of armies in the territory
 */
int Territory::getNumberOfArmies() const {
    return numberOfArmies;
}

/**
 * @return the current Owner of the territory
 */
const string &Territory::getOwner() const {
    return owner;
}

/**
 * Adds a Territory to the object Territory's neighbours
 * @param territory a pointer to the territory to be added
 */
void Territory::addNeighbour(Territory *territory) {
    neighbours.emplace_back(territory);
}

/**
 * @return the X-coordinate of the territory
 */
int Territory::getX() const {
    return x;
}

/**
 * @return the Y-coordinate of the territory
 */
int Territory::getY() const {
    return y;
}

/**
 * Copy constructor for the Territory class
 */
Territory::Territory(const Territory& territory) : id(territory.id), name(territory.name), x(territory.x), y(territory.y), owner(territory.owner) {
    numberOfArmies = 0;

    continent = territory.continent;
    neighbours = territory.neighbours;
}

/**
 * Stream insertion operator for the Territory class
 */
std::ostream &operator<<(std::ostream &out, const Territory *territory) {
    out << "Country " << territory->getId() << " of name " << territory->getName();
    out << " of continent " << territory->getContinent()->getName();
    out << " and x,y of " << territory->getX() << "," << territory->getY();
    return out;
}

///**************************///
/// Map Class Implementation ///
///**************************///

/**
 * Determines whether the given vector of Territories forms a connected graph, where the territories are nodes of the graph.
 * Performs this check using a DFS-based traversal of the graph
 * @param territories the vector of territories
 * @return true if the territories form a connected graph
 */
bool Map::isConnected(const vector<Territory *>& territories) {
    // Initialize empty list of territories visited during the traversal
    list<int> visitedTerritoryIDs;

    // Do a DFS-based traversal, starting from the first item of the territories
    dfs(territories.front(), territories, visitedTerritoryIDs);

    // Expected result of dfs: all territories from the provided vector have been visited
    // if all territories in the graph have been visited, then graph is connected

    // For each territory in the provided graph
    for (const auto &territory : territories) {
        // Check if the territory has been visited by searching for it in the visitedTerritoryIDs list
        auto isVisited = std::find(visitedTerritoryIDs.begin(), visitedTerritoryIDs.end(), territory->getId());
        if (isVisited == std::end(visitedTerritoryIDs)) {
            // If the search reaches the end of visitedTerritoryIDs, then the territory was not visited
            cerr << "Territory " << territory->getId() << " has not been visited!" << endl;
            return false;
        }
    }

    return true;
}

/**
 * Performs a depth-first traversal of the graph consisting of the provided collection of territories, starting from a
 * given territory. As new territories are visited during the traversal, the list of visited territories is appended with
 * the newly visited territories.
 * @param currentTerritory The currently visited territory
 * @param territories A vector of territories forming the graph
 * @param visitedTerritories The updated list of visited territories
 */
void Map::dfs(Territory *const &currentTerritory, const vector<Territory *> &territories, list<int> &visitedTerritories) {
    // Check if the visitedTerritories list contains the current territory, i.e. the current territory has been already visited
    // during the traversal
    auto isAlreadyVisited = std::find(visitedTerritories.begin(), visitedTerritories.end(), currentTerritory->getId());
    if (isAlreadyVisited != std::end(visitedTerritories)) {
        // Already visited node, no point visiting its adjacent nodes
        return;
    }

    // Current territory was not previously visited, so now add it to visitedTerritories
    visitedTerritories.emplace_back(currentTerritory->getId());

    // Recursively call dfs on all neighbours of the current territory that are also in the graph (which may be a subgraph)
    for (const auto &neighbour : currentTerritory->getNeighbours()) {
        auto isInTerritories = std::find(territories.begin(), territories.end(), neighbour);
        if (isInTerritories != std::end(territories)){
            dfs(neighbour, territories, visitedTerritories);
        }
    }
}

/**
 * Validates the Map structure according to a series of validation steps, namely:
 *
 * a) check no continents are empty;
 * b) each country belongs to one and only one continent;
 * c) the map represents a connected graph;
 * d) each continent is a connected subgraph of the "global" graph
 */
void Map::validate() {
    // Validation step: check no continents are empty
    // if continent is empty, map is invalid --> throw an exception
    cout << "Validating map..." << endl;
    for (const auto &continent : continents) {
        if (continent->isEmpty()) {
            cerr << "Continent " << continent->getName() << " does not have a country" << endl;
            throw;
        }
    }
    cout << "Success! No continents are empty!" << endl;

    // Validation step: requirement 3) each country belongs to one and only one continent
    // Create a vector of zeros matching the size of the territories list, representing the number of times a territory
    // appears as a territory among all continents
    vector<int> territoriesTaken(territories.size(), 0);

    for (const auto &continent : continents) {
        for (const auto &territory : continent->getTerritories()) {
            // increment the value associated to the territory
            int &lookupValue = territoriesTaken.at(territory->getId() - 1);
            lookupValue++;
        }
    }

    // All territories must be exactly visited once -- i.e. the territoriesTaken vector must have all 1's
    for (const auto &numberOfTimesVisited : territoriesTaken) {
        if (numberOfTimesVisited != 1) {
            cerr << "Territory must belong to one and only one continent, found " << numberOfTimesVisited;
            throw;
        }
    }
    cout << "Success! No continents share the same territory!" << endl;

    // Validation Step: requirement 1) a map is a connected graph
    // Test Map is connected graph
    cout << "Checking for connectedness of whole map..." << endl;
    if (!isConnected(territories)) {
        cerr << "ERROR! Map is not connected!" << endl;
        throw;
    }
    cout << "Success! The global map is connected!" << endl;

    // Validation Step: requirement 2) a continent is a connected subgraph
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

/**
 * Adds a given territory to the map
 * @param territory the territory
 */
void Map::addTerritory(Territory* territory) {
    territories.emplace_back(territory);
}

/**
 * @param i the territory ID
 * @return the territory corresponding to the territory ID
 */
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
 * @return the number of territories in the Map
 */
unsigned int Map::getSize() {
    return territories.size();
}

/**
 * Generates a representation of the Map in a Mermaid.js compliant flowchart.
 * The resulting text can be viewed at http://mermaid.live, or any editor that supports Mermaid.js
 * @return the string representation of the map
 */
string Map::toMermaid() {
    // First line is fixed -- "flowchart TB" for top-bottom flowchart
    string mermaid = "\nflowchart TB\n";

    // Next, define each territory in a separate subgraph, line by line
    // e.g.
    // subgraph SUBGRAPH_NAME
    // nodeA
    // nodeB
    // end

    // Each continent is a subgraph
    for (const auto &continent : continents) {
        // Set the name of the subgraph as "CONTINENT_NAME: ARMY_BONUS_NUMBER"
        mermaid.append("subgraph " + continent->getName() + ": " + std::to_string(continent->getArmyBonusNumber()) + "\n");

        // List each node in the subgraph, line by line
        // These are all the territories under a given continent
        for (const auto &territory : continent->getTerritories()) {
            mermaid.append(territory->getName() + "_" + std::to_string(territory->getId()) + "\n");
        }
        mermaid.append("end\n");
    }

    // Finally, list all the connectors between each node
    // Iterate through each pair of neighbouring territories
    for (const auto &territory : territories) {
        for (const auto &neighbour : territory->getNeighbours()) {
            // To avoid creating two connectors for the same pair of territories (due to being mutually in each other's
            // adjacency list), only draw one of the two by skipping when the territory in the adjacency list has an ID
            // value greater than the "origin" territory
            if (territory->getId() < neighbour->getId()) {
                mermaid.append(territory->getName() + "_" + std::to_string(territory->getId()) + "---" + neighbour->getName() + "_" + std::to_string(neighbour->getId()) + "\n");
            }
        }
    }
    return mermaid;
}

/**
 * Add continent to Map's vector of continents
 * @param continent the continent
 */
void Map::addContinent(Continent *continent) {
    continents.emplace_back(continent);
}

/**
 * Gets the continent according to its continent ID (as shown in the map txt file)
 * @param continentID the continent ID
 * @return the continent
 */
Continent *Map::getContinentByID(int continentID) {
    return continents[continentID - 1];
}

/**
 * @return the number of continents in the Map
 */
unsigned int Map::getContinentsSize() {
    return continents.size();
}

/**
 * Constructs a new map
 * @param name the common name given to the map
 */
Map::Map(string name) : name(name) {}

/**
 * Copy constructor for Map class
 */
Map::Map(const Map& map) : name(map.name) {
    territories = map.territories;
    continents = map.continents;
}

/**
 * Stream insertion operator for the Map class
 */
std::ostream &operator<<(std::ostream &out, Map* map) {
    out << "Map of name " << map->name;

    // output the list of territories of the map
    out << ", with countries :[";
    for (const auto &territory : map->territories) {
        out << territory << endl;
    }

    // output the list of continents of the map
    out << "] and continents: [" << endl;
    for (const auto &continent : map->continents) {
        out << continent << endl;
    }
    out << "]" << endl;

    return out;
}

/**
 * Load a map from a given file, while making sure file is valid (i.e. can be fully parsed)
 * @param string the filename
 * @return the Map object
 */
Map* MapLoader::load(const string& filename) {
    //
    const vector<string> sections = {"files", "continents", "countries", "borders"};

    // Regular expressions precisely matching each row in the file
    // Using ECMAScript-like regex strings for better readability
    const std::regex sectionRegex(R"(^\[.*\]$)"); // e.g. [borders]
    const std::regex fileRegex(R"(^\S+ \S+$)", ECMAScript); // e.g. "pic canada_pic.png"
    const std::regex continentRegex(R"(^\S+ \d+ \S+$)", ECMAScript); // e.g. "Ontario_and_Quebec 4 cyan"
    const std::regex countryRegex(R"(^\d+ \S+ \d+ \d+ \d+$)", ECMAScript); // e.g. "6 Quebec-North 2 430 261"
    const std::regex borderRegex(R"(^\d+( \d+)+$)", ECMAScript); // e.g. "7 4 5 6 8 11"

    // Create new map object to be used for parsing
    Map* gameMap = new Map("myMap");

    // Some text appears above the first section, call this "head"
    string currentSection = "head";

    // Keep two counters to keep track of the expected ID sequenced for countries and border
    int currentCountryIndex = 0;
    int currentBorderIndex = 0;

    // Input file stream opened using provided filename
    ifstream file(filename);
    file.open(MAPS_DIR + filename);

    // Iterate through the map file one line at a time
    string line;
    while (std::getline(file, line)) {
//        cout << line << endl;

        // skip lines starting with ';' as well as empty lines
        if (line.empty() || line.front() == ';') {
//            cout << "\tSkip!" << endl;
            continue;
        }

        // check if the line indicates the beginning of a new section in the file
        if (std::regex_match(line, sectionRegex)) {
            // TODO: Make sure sections are sequential
            // extract section name by removing enclosing square brackets (first and last character of line)
            string sectionName = line.substr(1, line.length()-2);

            // Make sure the section name is among the known section names
            if (find(sections.begin(), sections.end(), sectionName) != sections.end()) {
//                cout << "\tFound new section: " << sectionName << "!" << endl;

                // Set newly encountered section as the current section
                currentSection = sectionName;
            } else {
                throw std::runtime_error("Error parsing file: found unrecognized section " + sectionName);
            }
        } else {
            // line remains in existing section
            // identify section using switch statement
            switch (getSectionFromString(currentSection)) {
                case HEAD:
                    // No lines should be read in the head
                    throw std::runtime_error("Found unknown line in head: " + line);
                case FILES:
                    // parse file line as long as regex matches
                    if (std::regex_match(line, fileRegex)) {
                        // match -- parse file line
                        // FIXME: Currently this section is not yet to be parsed
                    } else {
                        throw std::runtime_error("File line doesn't meet expected format : \"" + line + "\"");
                    }
                    break;
                case CONTINENTS:
                    if (std::regex_match(line, continentRegex)) {
                        // match -- parse continent line

                        // Using a stringstream, which automatically tokenizes space-delimited strings
                        std::stringstream ss;

                        // Pass line into stringstream
                        ss << line;

                        // Map each element in the stream to an appropriate object (informed by the regex matching)
                        string name;
                        int bonusNumber;
                        string color;
                        ss >> name >> bonusNumber >> color;

                        // Create new continent from mapped data
                        auto* continent = new Continent(name, color, bonusNumber);
//                        cout << continent << endl;

                        // Add continent to the map
                        gameMap->addContinent(continent);
                    } else {
                        throw std::runtime_error("Continent line doesn't meet expected format : \"" + line + "\"");
                    }
                    break;
                case COUNTRIES:
                    if (std::regex_match(line, countryRegex)) {
                        // match -- parse country line

                        // Using string stream to map string to Territory object
                        std::stringstream ss;
                        ss << line;

                        int countryID;
                        string countryName;
                        int continentID;
                        int xCoordinate;
                        int yCoordinate;
                        string color;
                        ss >> countryID >> countryName >> continentID >> xCoordinate >> yCoordinate;

                        // Additional validation to prevent bad behaviour: Ensure country id increases sequentially from 1
                        if (currentCountryIndex + 1 != countryID) {
                            throw std::runtime_error("Expected country id " + std::to_string(currentCountryIndex + 1) + ", got " +
                                                             std::to_string(countryID));
                        }

                        // Country ID is valid -- increment the counter to get the next expected country index
                        currentCountryIndex++;

                        // Validation step: ensure continent ID in the line exists
                        // continentID ranges from 1 to (number of continents in map), inclusively
                        if (continentID <= 0 || continentID > gameMap->getContinentsSize()) {
                            throw std::runtime_error("Country has an invalid continent id: " + std::to_string(continentID));
                        }

//                        cout << "\tFound country " << countryName << " of continent id " << continentID << " and x,y of " << xCoordinate << "," << yCoordinate << endl;

                        // All parsing and validation complete, create territory from mapped data
                        auto* territory = new Territory(countryID, countryName, xCoordinate, yCoordinate, gameMap->getContinentByID(continentID));
//                        cout << territory << endl;

                        // Assign territory to the continent
                        gameMap->getContinentByID(continentID)->addTerritory(territory);

                        // Add territory to map
                        gameMap->addTerritory(territory);
                    } else {
                        throw std::runtime_error("Country line doesn't meet expected format : \"" + line + "\"");
                    }
                    break;
                case BORDERS:
                    if (std::regex_match(line, borderRegex)) {
                        // match -- parse border line
                        // Using string stream to map string to Country object
                        std::stringstream ss;
                        ss << line;

                        // Get first column, this is the "origin" of each connection between nodes
                        int originCountryID;
                        ss >> originCountryID;

                        // Validate -- first column must be sequentially increasing in the borders collection
                        if (currentBorderIndex + 1 != originCountryID) {
                            throw std::runtime_error("Expected origin country id " + std::to_string(currentBorderIndex + 1) + ", got " +
                                                             std::to_string(originCountryID));
                        }

                        // First column is indeed incrementing -- increment the counter to get the next expected value
                        currentBorderIndex++;

                        int currentNeighbourID;
                        while (ss >> currentNeighbourID) {
                            // Validate -- neighbour must exist in map
                            if (currentNeighbourID <= 0 || currentNeighbourID > gameMap->getSize()) {
                                throw std::runtime_error("No neighbour territory in map with ID " + std::to_string(currentNeighbourID));
                            }

                            // Neighbour exists, add edge between the territories
                            gameMap->addEdge(originCountryID, currentNeighbourID);
//                                cout << "\tAdded edge between countries " << originCountryID << " and " << currentNeighbourID << endl;
                        }
                    } else {
                        throw std::runtime_error("Border line doesn't meet expected format : \"" + line + "\"");
                    }
                    break;
                case DEFAULT:
                default:
                    throw std::runtime_error("Default line doesn't meet expected format : \"" + line + "\"");
            }
        }
    }

    cout << "Printout of Map:" << endl;
    cout << gameMap << endl;

    file.close();

    cout << "Finished reading file into Map" << endl;
    return gameMap;
}
