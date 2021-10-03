#include "cards.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>


// Implementation of the Cards, Hands and Deck classes

// Default constructor of the Card class
Card::Card() {
    // Type initialization
    type = new Type(static_cast<Type>(rand() % 5));
}

// Constructor
Card::Card(const Type& cardType) {
    type = new Type(cardType);
}

// Copy constructor
Card::Card(const Card &card) {
    this->type = new Type(*card.type);
}

// Assignment operator
Card& Card::operator=(const Card& card) {
    this->type = new Type(*card.type);
    return *this;
}

// Stream insertion operator
istream& operator >> (istream &in, Card &c) {
//    int card_num;
//    cout << "Choose a card type number from the following choices:\n 1) bomb\n 2) reinforcement\n 3) blockade\n 4) airlift\n 5) diplomacy" << endl;
//    cin >> card_num;
//    Type type = static_cast<Type>(card_num);
//    c.setType(type);
//    in >> c.type;
    return in;// how can i do this ?
}

// Stream output operator
ostream& operator << (ostream& os, const Card &c){
    return os << "The card type is: " << *c.type << endl;
}

// Destructor
Card::~Card() {
    delete type;
    type = NULL;
}

// Getter for card type
Type *Card::getType() const {
    return type;
}

// Setter for card type
void Card::setType(const Type& type) {
    this->type = new Type(type);
}

void play(const Hand& hand, const Deck& deck) {
    // Create an order with this function

    // Remove it from hand, put it in deck
}

// Default constructor of the Deck class
Deck::Deck() {
    warzoneCards = new vector<Card>();
}

// Constructor
Deck::Deck(vector<Card> cards) {
    warzoneCards = new vector<Card>(cards);
}

// Copy constructor
Deck::Deck(const Deck &deck) {
    this->warzoneCards = new vector<Card>(*(deck.warzoneCards));
}

// Assignment operator
Deck& Deck::operator=(const Deck &deck) {
    warzoneCards = new vector<Card>(*(deck.warzoneCards));
    return *this;
}

// Stream insertion operator for the Deck class
istream& operator >> (istream &in, Deck& d){
    int num_cards;
    cout << "How many cards do you want to add ?" << endl;
    cin >> num_cards;
    cout << "For each card, select the number of the type you want to assign it to" << endl;
    cout << "0: bomb\n 1: reinforcement\n 2: blockade\n 3: airlift\n 4: diplomacy" << endl;
    for(int i (0); i < num_cards; i++){
        int type_num;
        cout << "For card number " << (i+1) << endl;
        cin >> type_num;
        Type type = static_cast<Type>(type_num);
        Card card(type);
        d.warzoneCards->emplace_back(card);
    }
    in >> reinterpret_cast<Card &>(d.warzoneCards);
    return in;
    // How do I do this ?
}

// Stream insertion operator for the Deck class
ostream& operator << (ostream &os, const Deck& d){
    os << "The deck's warzone cards are the following: " << endl;
    for(Card card : *d.warzoneCards){
        os << card;
    }
    return os;
}

// Destructor for the Deck class
Deck::~Deck() {
    delete warzoneCards;
    warzoneCards = NULL;
}

// Getter for the cards in the deck
vector<Card> *Deck::getWarzoneCards() const {
    return warzoneCards;
}

// Setter for the cards in the deck
void Deck::setWarzoneCards(const vector<Card> &cards) {
    this->warzoneCards = new vector<Card>(cards);
}

// This function allows a player to draw a card from the deck and to put it in their hand
void Deck::draw(const Hand& hand) {
    int card_num = pickCard();
    Card picked_card { this->warzoneCards->at(card_num) };

    this->warzoneCards->erase(warzoneCards->begin() + card_num);
    hand.getHandsCards()->emplace_back(picked_card);

    cout << card_num << endl;
    cout << picked_card << endl;
}

int Deck::pickCard() {
    int randomIndex = rand() % this->warzoneCards->size();
    return randomIndex;
}

// Default constructor for the Hand method
Hand::Hand() {
    handsCards = new vector<Card>();
}

// Constructor
Hand::Hand(vector<Card> cards) {
    handsCards = new vector<Card>(cards);
}

Hand::Hand(const Hand &hand) {
    this->handsCards = new vector<Card>(*(hand.handsCards));
}

Hand& Hand::operator=(const Hand &hand) {
    this->handsCards = new vector<Card>(*(hand.handsCards));
    return *this;
}

// Stream insertion operation
istream& operator >> (istream &in, Hand& h){
    // what to do
    return in;
}

// Stream output operator
ostream& operator << (ostream &os, const Hand& h) {
    os << "The hand's warzone cards are the following: " << endl;
    for(Card card : *h.handsCards){
        os << card;
    }
    return os;
}

// Destructor
Hand::~Hand() {
    delete handsCards;
}

// Getter for handsCards
vector<Card> *Hand::getHandsCards() const {
    return handsCards;
}

// Setter for handsCards
void Hand::setWarzoneCards(const vector<Card> &cards) {
    this->handsCards = new vector<Card>(cards);
}