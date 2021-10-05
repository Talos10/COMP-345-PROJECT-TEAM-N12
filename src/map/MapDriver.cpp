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
    MapLoader mapLoader = MapLoader();
    string filename = "demo-map.txt";
    Map map = mapLoader.load(filename);
    // Call MAp validate
}
