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
    if(this->type != card.type) {
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
    vector<Card> &hand_cards (*(player.getHand())->getHandsCards());
    Card card (*this);
    bool found (false);

    // Create an iterator that will point to the same card in the hand as the current card being played
    vector<Card>::iterator it = std::find(hand_cards.begin(), hand_cards.end(), card);

    // If the card is found, delete it
    if(it != hand_cards.end()){
        hand_cards.erase(it);
        found = true;
    }

    // Put the card in the back of the deck;
    if(found)
        deck.getWarzoneCards()->insert(deck.getWarzoneCards()->begin(), card);
}

//---Implementation of the Deck class---

// Default constructor of the Deck class which initializes an empty vector of Card
Deck::Deck() {
    warzoneCards = new vector<Card>();
}

// Constructor with a vector of Card as the only parameter
Deck::Deck(const vector<Card>& cards) {
    warzoneCards = new vector<Card>(cards);
}

// Copy constructor
Deck::Deck(const Deck &deck) {
    this->warzoneCards = new vector<Card>(*(deck.warzoneCards));
}

// Assignment operator
Deck& Deck::operator=(const Deck& deck) {
    if(this->warzoneCards != deck.warzoneCards){
        delete this->warzoneCards;
        this->warzoneCards = new vector<Card>(*(deck.warzoneCards));
    }
    return *this;
}

// Stream insertion operator for the Deck class
ostream& operator << (ostream &os, const Deck& d){
    os << "The deck's warzone cards are the following: " << endl;
    for(Card& card : *d.warzoneCards){
        os << card;
    }
    return os;
}

// Destructor for the Deck class
Deck::~Deck() {
    delete warzoneCards;
    warzoneCards = nullptr;
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
Card& Deck::draw(const Hand& hand) {
    hand.getHandsCards()->emplace_back(this->warzoneCards->back()); // put deck card in hand
    this->warzoneCards->pop_back(); // remove last card from deck (the one we put in the hand)
    return hand.getHandsCards()->back();
}

//---Implementation of the Hand class---

// Default constructor for the Hand method which initializes an empty vector of Card
Hand::Hand() {
    handsCards = new vector<Card>();
}

// Constructor that initializes an empty vector of Card
Hand::Hand(const vector<Card>& cards) {
    handsCards = new vector<Card>(cards);
}

// Constructor that passes a vector of Card as the only parameter
Hand::Hand(const Hand &hand) {
    this->handsCards = new vector<Card>(*(hand.handsCards));
}

// Assignment operator
Hand& Hand::operator=(const Hand& hand) {
    if(this->handsCards != hand.handsCards){
        delete this->handsCards;
        this->handsCards = new vector<Card>(*(hand.handsCards));
    }
    return *this;
}

// Stream insertion operator for the Hand class
ostream& operator << (ostream &os, const Hand& h) {
    os << "The hand's warzone cards are the following: " << endl;
    for(Card& card : *h.handsCards){
        os << card;
    }
    return os;
}

// Destructor
Hand::~Hand() {
    delete handsCards;
    handsCards = nullptr;
}

// Getter for handsCards
vector<Card>* Hand::getHandsCards() const {
    return handsCards;
}

// Setter for handsCards
void Hand::setWarzoneCards(const vector<Card> &cards) {
    this->handsCards = new vector<Card>(cards);
}

void card_driver() {
    cout << "### Running Cards driver! ###" << endl;

    srand(time(nullptr)); //Initialize random seed
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

    Player player;
    player.setHand(handObj);

    cout << deck << endl;
    cout << player << endl;

    vector<Card>& hands_card (*(player.getHand())->getHandsCards());

    while (begin(hands_card) != end(hands_card)) {
        player.getHand()->getHandsCards()->at(0).play(deck, player);
    }

    cout << deck << endl;
    cout << player << endl;
}