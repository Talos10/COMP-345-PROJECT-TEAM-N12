#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"
#include "game_log/log_observer.h"
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]) {
    cout << "Running main driver!" << endl;

    int classDemo = 4;

    string filename1 = "canada.txt";
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
            game_engine_driver(argv[2]); // Read commands from file commands_play_1.txt.
            break;
        case 6:
            game_engine_driver(argv[3]); // Read commands from file commands_play_2.txt.
            break;
        case 7:
            game_engine_driver(argv[4]); // Read commands from file commands_play_fail_1.txt.
            break;
        case 8:
            game_engine_driver(argv[5]); // Read commands from file commands_play_fail_2.txt.
            break;
        default:
            break;
    }

    return 0;
}
