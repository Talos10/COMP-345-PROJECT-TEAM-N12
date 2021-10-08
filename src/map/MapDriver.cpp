//
// Created by donya on 10/2/2021.
//

#include "MapDriver.h"
#include "map.h"
#include <iostream>
using std::cout; using std::cerr;
using std::endl;

void MapDriver::map_driver_run() {
    string filename = "canada-map.txt";

    try {
        // Call Map Loader on file, retrieve map
        Map* map = MapLoader::load(filename);

        // Print out representation of the map, even if unvalid
        cout << "**********Generating mermaid representation of map...****************" << endl;
        string mermaid = map->toMermaid();
        cout << mermaid;
        cout << "**********DONE****************" << endl;

        // Run validation
        map->validate();
    } catch (std::runtime_error &exp) {
        // Catch all exceptions defined as runtime errors
        cerr << exp.what() << endl;
    }
}
