//
// Created by patri on 10/1/2021.
//

#include "cards.h"
#include <time.h>

void main_card() {
    cout << "Running main driver!" << endl;

    srand(time(NULL)); //Initialize random seed
    Card c1;
    Card c2;
    Card c3;
    vector<Card> ayre;
    ayre.emplace_back(c1);
    ayre.emplace_back(c2);
    ayre.emplace_back(c3);
    Deck d1 (ayre);

    Deck d2 (d1);

    Deck d3 = d1;

    Deck d4;
    d4 = d1;

//    cout << d1 << endl;
//    cout << d2 << endl;
//    cout << d3 << endl;
//    cout << d4 << endl;

    Card c4;
    Card c5;
    Card c6;
    vector<Card> zob;
    zob.emplace_back(c4);
    zob.emplace_back(c5);
    zob.emplace_back(c6);
    Deck d5 {zob};
    Hand h1 {ayre};

    cout << d5 << endl;
    cout << h1 << endl;

    d5.draw(h1);

    cout << d5 << endl;
    cout << h1 << endl;
}
