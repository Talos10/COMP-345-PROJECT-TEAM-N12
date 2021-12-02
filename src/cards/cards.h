#pragma once

#include "orders/orders.h"
#include "player/player.h"
#include <iostream>
#include <vector>
using namespace std;

// Enumeration for the type of cards
enum Type {
    bomb,
    reinforcement,
    blockade,
    airlift,
    diplomacy
};

// Forward declarations
class Deck;
class Hand;
class Card;
class Player;
class Order;
class Territory;
class LogObserver;

// This class represents the cards that will be played during the game, they can each have one of the 5 types
class Card {
private:
    // Initialization of type enum
    Type *type;
public:
    // Default constructor
    Card();

    // Constructor
    explicit Card(const Type& cardType);

    // Copy constructor
    Card(const Card& card);

    // Assignment operator
    Card& operator=(const Card& card);

    // Insertion operator
    friend ostream& operator << (ostream& os, const Card& c);

    // Destructor
    ~Card();

    // Getter for enum
    [[nodiscard]] Type* getType() const;

    // Setter for enum
    void setType(const Type& type);

    // A function which lets a player create orders. If a card has been played, remove it from the hands and put it back in the deck
    void play(Deck &deck, Player &player, tuple<Territory *, Territory *, string> *orderInfo, LogObserver& log);

    // A function that checks if two objects are the same
    friend bool operator == (const Card &c1, const Card &c2);
};

// This class represents the set of cards that will be held by the players during the gme
class Hand {
private:
    // Each player contains a finite amount of cards in their hand.
    vector<Card*>* handsCards;
public:
    // Default constructor
    Hand();

    // Constructor
    explicit Hand(const vector<Card*> &cards);

    // Copy constructor
    Hand(const Hand& hand);

    // Assignment operator
    Hand& operator=(const Hand& hand);

    // Insertion operator
    friend ostream& operator << (ostream& os, const Hand& h);

    //Destructor
    ~Hand();

    // Getter for handsCards
    [[nodiscard]] vector<Card*>* getHandsCards() const;

    // Setter for handsCards
    void setHandCards(const vector<Card*> &cards);
};

// This class represents the set of warzone cards where all players draw from when it's their turn to play
class Deck {
private:
    // The deck contains a finite amount of cards
    vector<Card*>* warzoneCards;
public:
    // Default constructor
    Deck();

    // Constructor
    explicit Deck(const vector<Card*>& cards);

    // Parameterized Constructor
    explicit Deck(const int numOfCards);

    // Copy constructor
    Deck(const Deck& deck);

    // Assignment operator
    Deck& operator=(const Deck& deck);

    // Insertion operator
    friend ostream &operator<<(ostream &os, const Deck &d);

    //Destructor
    ~Deck();

    // Getter for warzone cards
    [[nodiscard]] vector<Card *> *getWarzoneCards() const;

    // Setter for warzone cards
    void setDeckCards(const vector<Card *> &cards);

    void setDeck(const Deck &deck);

    // A function which lets a player draw a card from the deck and places it in their hand
    void draw(const Hand &hand);
};

void card_driver();