#include <iostream>
#include "game_engine/game_engine.h"
#include "orders/Orders.h"

using namespace std;

int main() {
    cout << "Running main driver!" << endl;

    game_engine_driver();
    orders_driver();


    return 0;
}
