#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"
#include "cards/cards.h"
#include <time.h>

using namespace std;

int main() {
    cout << "Running main driver!" << endl;

    //game_engine_driver();

    player_driver();

    return 0;
}
