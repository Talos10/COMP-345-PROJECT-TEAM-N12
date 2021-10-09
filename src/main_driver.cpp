#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"

using namespace std;

int main() {
    cout << "Running main driver!" << endl;

    int classDemo = 0;

    switch(classDemo) {
        case 0:
            map_driver();
            break;
        case 1:
            card_driver();
            break;
        case 2:
            orders_driver();
            break;
        case 3:
            player_driver();
            break;
        case 4:
            game_engine_driver();
            break;
    }

    return 0;
}
