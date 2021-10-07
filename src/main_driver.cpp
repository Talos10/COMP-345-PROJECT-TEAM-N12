#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"
#include "cards/cards.h"
#include <time.h>
#include "orders/Orders.h"
#include "map/MapDriver.h"


using namespace std;

int main() {
    cout << "Running map driver!" << endl;

    //game_engine_driver();
    player_driver();
    //orders_driver();

    //MapDriver mapDriver = MapDriver();
    //mapDriver.map_driver_run();


    return 0;
}
