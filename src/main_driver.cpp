#include <iostream>
#include "game_engine/game_engine.h"
#include "player/player.h"
#include "game_log/log_observer.h"
#include <fstream>

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

void logger_driver() {
//    Order* order1 = new Deploy();
//    Order* order2 = new Airlift();
//    vector<Subject*> *subjectsList = new vector<Subject*>();
//    subjectsList->push_back(order1);
//    subjectsList->push_back(order2);
//    LogObserver *log = new LogObserver(subjectsList);
//    order1->execute();
//    order2->execute();
//    cout << "order1: " << order1->getAllObservers() << endl;
//    cout << "order2: " << order2->getAllObservers() << endl;
//
//    // LogObserver *log2 = new LogObserver();
//    Order* order3 = new Advance();
//    log->AddSubject(*order3);
//    Order* order4 = new Blockade();
//    log->AddSubject(*order4);
//    Order* order5 = new Bomb();
//    log->AddSubject(*order5);
//    Order* order6 = new Negotiate();
//    log->AddSubject(*order6);
//    order3->execute();
//    order4->execute();
//    order5->execute();
//    order6->execute();
//    cout << "order3: " << order3->getAllObservers() << endl;
//    cout << "order4: " << order4->getAllObservers() << endl;
//    cout << "order5: " << order5->getAllObservers() << endl;
//    OrdersList* orders_list = new OrdersList();
//    log->AddSubject(*orders_list);
//    orders_list->addOrder(order1);
//    orders_list->addOrder(order2);
//    orders_list->addOrder(order3);
//    orders_list->addOrder(order4);
//    orders_list->addOrder(order5);
//    orders_list->addOrder(order6);
//
//    GameEngine *ge = new GameEngine("gameStart");
//    log->AddSubject(*ge);
//    ge->transition("bro");
//    CommandProcessor *cp = new CommandProcessor();
//    log->AddSubject(*cp);
//    Command *c1 = new Command("gameStart");
//    log->AddSubject(*c1);
//    Command& c2 = *cp->getCommand(*ge);
//    log->AddSubject(c2);
//    c1->saveEffect("bro", true);
}
