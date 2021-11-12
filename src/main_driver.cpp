#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Running main driver!" << endl;

    int classDemo = 5;

    string filename1 = "canada-map.txt";
    string filename2 = "demo-map.txt";
    string filename3 = "canada-map-not-connected.txt";

    switch (classDemo) {
        case 0:
            map_driver(filename1);
            break;
        case 1:
            card_driver();
            break;
        case 2:
            orders_driver();
            break;
        case 3:
            player_driver(filename1);
            break;
        case 4:
            game_engine_driver(argv[1]); // Read commands from console.
            break;
        case 5:
            game_engine_driver(argv[2]); // Read commands from file commands.txt.
            break;
        default:
            break;
    }

    return 0;
}
