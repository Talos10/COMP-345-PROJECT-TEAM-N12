#include <iostream>
#include "game_engine/game_engine.h"
#include "map/MapDriver.h"

using namespace std;

int main() {
    cout << "Running map driver!" << endl;

    MapDriver mapDriver = MapDriver();
    mapDriver.map_driver_run();

    return 0;
}
