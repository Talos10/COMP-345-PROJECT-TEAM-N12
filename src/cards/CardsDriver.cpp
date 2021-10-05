//
// Created by patri on 10/1/2021.
//

#include "cards.h"
#include <time.h>
#include <random>

void main_card() {
    cout << "Running main driver!" << endl;

    srand(time(NULL)); //Initialize random seed
    Card c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;

    vector<Card> deck_template;

    deck_template.emplace_back(c1);
    deck_template.emplace_back(c2);
    deck_template.emplace_back(c3);
    deck_template.emplace_back(c4);
    deck_template.emplace_back(c5);
    deck_template.emplace_back(c6);
    deck_template.emplace_back(c7);
    deck_template.emplace_back(c8);
    deck_template.emplace_back(c9);
    deck_template.emplace_back(c10);

    Deck deck (deck_template);

    Hand handObj;
    cout << deck << endl;
    cout << handObj << endl;

    deck.draw(handObj);
    deck.draw(handObj);
    deck.draw(handObj);
    deck.draw(handObj);

    cout << deck << endl;
    cout << handObj << endl;

//    c1.play(deck, handObj);
//    c2.play(deck, handObj);
//    c3.play(deck, handObj);
//    c4.play(deck, handObj);
//    c5.play(deck, handObj);
//    c6.play(deck, handObj);
//    c7.play(deck, handObj);
//    c8.play(deck, handObj);
//    c9.play(deck, handObj);
//    c10.play(deck, handObj);

//for(Card& card: *handObj.getHandsCards()){
//    //cout << handObj.getHandsCards()->size() << endl;
//    card.play(deck, handObj);
//}
    c1.play(deck, handObj);
    c2.play(deck, handObj);
    c3.play(deck, handObj);
    c4.play(deck, handObj);
    c5.play(deck, handObj);
    c6.play(deck, handObj);
    c7.play(deck, handObj);
    c8.play(deck, handObj);
    c9.play(deck, handObj);
    c10.play(deck, handObj);

    cout << deck << endl;
    cout << handObj << endl;

}
