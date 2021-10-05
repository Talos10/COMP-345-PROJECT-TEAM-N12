#include "map.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>

#define MAPS_DIR "../maps/"
using std::cout, std::endl, std::cerr;
using std::ifstream;
using std::find;

vector<Continent> continents;

enum section {HEAD, FILES, CONTINENTS, DEFAULT};
section getSectionFromString(const string& str) {
    if (str == "head") return HEAD;
    if (str == "files") return FILES;
    if (str == "continents") return CONTINENTS;
    return DEFAULT;
}

void Map::validate() {
//TODO validate() Definition
}

Continent::Continent(int index, string name, string color, int armyBonusNumber) : index(index),
                                                                                                name(std::move(name)),
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

    const std::regex sectionRegex("\\[.*\\]");
    const std::regex fileRegex("[a-z]+ [A-Za-z_.]+");
    const std::regex continentRegex("[A-Za-z_]+ [:digit:]+ [A-Za-z_]+");
//    const std::regex countryRegex("+ \\d+ [A-Za-z_]+");

    string currentSection = "head";

    while (std::getline(file, line)) {
        cout << line << endl;

        // ignore lines starting with ; or empty lines
        if (line.empty() || line.front() == ';') {
            cout << "\tSkip!" << endl;
            continue;
        }

        // switch to new section if exists
        if (std::regex_match(line, sectionRegex)) {
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

                    } else {
                        cout << "\tContinent line doesn't meet expected format" << endl;
                    }
                    break;
                case DEFAULT:
                default:
                    cout << "\tShouldn't be here!" << endl;
                    break;
            }
        }

    }

    file.close();

    cout << "Finished reading file into Map";
    return Map();
}
