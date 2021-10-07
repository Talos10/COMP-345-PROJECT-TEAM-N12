//
// Created by donya on 10/2/2021.
//

#include "MapDriver.h"
#include "map.h"
#include <iostream>
using std::cout;
using std::endl;

void MapDriver::map_driver_run() {
    // Call Map Loader on file, retrieve map
    string filename = "canada-map.txt";
    Map* map = MapLoader::load(filename);
    string mermaid = map->toMermaid();
    cout << mermaid;
    // Call MAp validate
}
