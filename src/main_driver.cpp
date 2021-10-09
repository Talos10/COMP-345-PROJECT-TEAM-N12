#include <iostream>
#include "game_engine/game_engine.h"
#include "map/MapDriver.h"
#include "player/player.h"
#include "cards/cards.h"
#include <time.h>
#include "orders/Orders.h"

using namespace std;

int main() {
    cout << "Running map driver!" << endl;

    MapDriver mapDriver = MapDriver();
    mapDriver.map_driver_run();

    //game_engine_driver();
    //player_driver();
    //orders_driver();
    //card_driver();

    return 0;
}
