#include "cards.h"
#include <ctime>
#include <algorithm>
using namespace std;

// ### This file represents the implementation of the Cards, Hands and Deck classes ###

//---Implementation of the Card class---

// Default constructor of the Card class which initializes a card type
Card::Card() {
    type = new Type(static_cast<Type>(rand() % 5));
}

// Constructor with a card type as the only parameter
Card::Card(const Type& cardType) {
    type = new Type(cardType);
}

// Copy constructor
Card::Card(const Card &card) {
    this->type = new Type(*card.type);
}

// Assignment operator
Card& Card::operator=(const Card& card) {
    if(this != &card) {
        delete this->type;
        this->type = new Type(*(card.type));
    }
    return *this;
}

// Stream insertion operator
ostream& operator << (ostream& os, const Card &c){
    return os << "The card type is: " << *c.type << endl;
}

// This function overrides the equality operator for two cards. Cards are considered equal if they have the same type
bool operator == (const Card &c1, const Card &c2){
    if(*c1.type == *c2.type)
        return true;
    return false;
}

// Destructor
Card::~Card() {
    delete type;
    type = nullptr;
}

// Getter for card type, returns a pointer of Type
Type* Card::getType() const {
    return type;
}

// Setter for card type
void Card::setType(const Type& card_type) {
    this->type = new Type(card_type);
}

// This function creates an order of the current card and adds it to a list of orders which is a parameter passed by reference.
// Then, it removes the card that was played from the hand which is a parameter passed by reference
// Finally, it adds the same card to the deck
void Card::play(Deck& deck, Player& player) {
    // Create an order, set the effect and the description, and push it in a list of orders
    string effect = "";
    string description = "This card has the type: ";
    switch (*this->getType()) {
        case 0:
            description += "bomb";
            effect = "bomb";
            break;
        case 1:
            description += "reinforcement";
            effect = "reinforcement";
            break;
        case 2:
            description += "blockade";
            effect = "blockade";
            break;
        case 3:
            description += "airlift";
            effect = "airlift";
            break;
        case 4:
            description += "diplomacy";
            effect = "diplomacy";
            break;
    }
    // Create an order with a description and an effect chosen by the card type
    Order *order = new Order();
    order->setDescription(description);
    order->setEffect(effect);

    // Put the created order in the list of orders
    player.issueOrder(order);


    // Create references for the content of the hand cards and the content of the card being played
    vector<Card*>* hand_cards = player.getHand()->getHandsCards();
    Card* card_address = this;

    // Create an iterator that will point to the same card in the hand as the current card being played
    vector<Card*>::iterator it = std::find(hand_cards->begin(), hand_cards->end(), card_address);

    // If the card is found, delete the reference to the hand card, and add the reference to the deck
    if(it != hand_cards->end()){
        hand_cards->erase(it);
        deck.getWarzoneCards()->insert(deck.getWarzoneCards()->begin(), card_address);
    }
}

//---Implementation of the Deck class---

// Default constructor of the Deck class which initializes an empty vector of Card
Deck::Deck() {
    this->warzoneCards = new vector<Card*>();
}

// Constructor with a vector of Card as the only parameter
Deck::Deck(const vector<Card*> &cards) {
    this->warzoneCards = new vector<Card*>(cards);
}

// Copy constructor
Deck::Deck(const Deck &deck) {
    for(int i = 0; i < deck.getWarzoneCards()->size(); i++) {
        Card* c = new Card(*(deck.warzoneCards->at(i)));
        this->warzoneCards->emplace_back(c);
    }
}

// Assignment operator
Deck& Deck::operator=(const Deck& deck) {
    if(this != &deck){
        // remove all cards in left hand side in case there are values
        for(int index = 0; index < this->warzoneCards->size(); index++){
            this->warzoneCards->pop_back();
        }
        // add all cards from right hand side into left side
        for(int i = 0; i < deck.warzoneCards->size(); i++){
            Card* c = new Card(*(deck.warzoneCards->at(i)));
            this->warzoneCards->emplace_back(c);
        }
    }
    return *this;
}

// Stream insertion operator for the Deck class
ostream& operator << (ostream &os, const Deck& d){
    os << "The deck contains " << d.warzoneCards->size() << " cards: " << endl;
    for(Card* card : *d.getWarzoneCards()){
        os << *card;
    }
    return os;
}

// Destructor for the Deck class
Deck::~Deck() {
    // delete each card on the heap
    for (Card* card : *this->warzoneCards){
        delete card;
    }
    this->warzoneCards->clear(); // delete all card pointers
    delete this->warzoneCards;
    this->warzoneCards = nullptr;
}

// Getter for the cards in the deck
vector<Card*>* Deck::getWarzoneCards() const {
    return this->warzoneCards;
}

// Setter for the cards in the deck
void Deck::setDeckCards(const vector<Card*> &cards) {
    this->warzoneCards = new vector<Card*>(cards);
}

// This function allows a player to draw a card from the deck and to put it in their hand
void Deck::draw(const Hand& hand) {
    int lastElementPosition = this->warzoneCards->size() - 1;
    Card* card = this->warzoneCards->back();

    hand.getHandsCards()->emplace_back(card); // put deck card in hand
    this->warzoneCards->erase(this->warzoneCards->begin() + lastElementPosition); // erase last pointer
}

//---Implementation of the Hand class---

// Default constructor for the Hand method which initializes an empty vector of Card
Hand::Hand() {
    handsCards = new vector<Card*>();
}

// Constructor with a vector of Card as the only parameter
Hand::Hand(const vector<Card*>& cards) {
    handsCards = new vector<Card*>(cards);
}

// Copy constructor
Hand::Hand(const Hand &hand) {
    for(int i = 0; i < hand.getHandsCards()->size(); i++) {
        Card* c = new Card(*(hand.handsCards->at(i)));
        this->handsCards->emplace_back(c);
    }
}

// Assignment operator
Hand& Hand::operator=(const Hand& hand) {
    if(this != &hand){
        // Delete all cards in left hand side in case there are already members
        for(int index = 0; index < this->handsCards->size(); index++) {
            this->handsCards->pop_back();
        }
        // add all cards from right hand side to left hand side
        for (int i = 0 ; i < hand.getHandsCards()->size(); i++) {
            Card* c = new Card(*(hand.handsCards->at(i)));
            this->handsCards->emplace_back(c);
        }
    }
    return *this;
}

// Stream insertion operator for the Hand class
ostream& operator << (ostream &os, const Hand& h) {
    os << "The hand contains " << h.handsCards->size() << " cards" << endl;
    for(Card* card : *h.handsCards){
        os << *card << endl;
    }
    return os;
}

// Destructor
Hand::~Hand() {
    // delete each card on the heap
    for (Card* card : *this->handsCards){
        delete card;
    }
    this->handsCards->clear(); // delete all card pointers
    delete this->handsCards;
    this->handsCards = nullptr;
}

// Getter for handsCards
vector<Card*>* Hand::getHandsCards() const {
    return this->handsCards;
}

// Setter for handsCards
void Hand::setHandCards(const vector<Card*> &cards) {
    this->handsCards = new vector<Card*>(cards);
}

void card_driver() {
    cout << "### Running Cards driver! ###" << endl;

    srand(time(nullptr)); //Initialize random seed

    // Create 10 cards
    Card* c1 = new Card();
    Card* c2 = new Card();
    Card* c3 = new Card();
    Card* c4 = new Card();
    Card* c5 = new Card();
    Card* c6 = new Card();
    Card* c7 = new Card();
    Card* c8 = new Card();
    Card* c9 = new Card();
    Card* c10 = new Card();

    // Add the cards to the deck
    vector<Card*> deck_template;
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

    // Put the values of the cards in the deck
    Deck deck (deck_template);

    // Create a player
    Player player;

    cout << deck << endl;
    cout << *player.getHand() << endl;

    // Draw 4 cards from the deck, by placing them in the player's hand
    deck.draw(*player.getHand());
    deck.draw(*player.getHand());
    deck.draw(*player.getHand());
    deck.draw(*player.getHand());

    // Print the deck, and the hand
    cout << deck << endl;
    cout << *player.getHand() << endl;

    // Play all the cards in the hand
    while (!player.getHand()->getHandsCards()->empty()) {
        player.getHand()->getHandsCards()->at(0)->play(deck, player);
    }

    // Print the deck, the hand and the orders
    cout << deck << endl;
    cout << *player.getHand() << endl;
    cout << *player.getOrders() << endl;
}