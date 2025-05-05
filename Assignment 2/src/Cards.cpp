#include "Cards.h"

//////////////////////////////////////////////////
// Card class function definitions
//////////////////////////////////////////////////

// Parameterized constructor
Card::Card(CardType cardType)
	: cardType{new CardType{cardType}} {
	std::cout << "Called Card constructor" << std::endl;
}

// Copy constructor
Card::Card(const Card& source) 
	: cardType{new CardType{*source.cardType}} {
}

// Destructor, deallocates memory for the pointer data member
Card::~Card() {
	delete cardType;
	std::cout << "Called Card destructor" << std::endl;
}

// Copy assignment operator overload
Card& Card::operator=(const Card& rhs) {
	if (this != &rhs) {
		delete cardType;
		cardType = new CardType{*rhs.cardType};
	}
	return *this;
}

// Equality operator overload
bool Card::operator==(const Card& rhs) const {
	return this->cardType == rhs.cardType;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Card& obj) {
	switch (obj.getCardType()) {
		case CardType::Bomb: return os << "Bomb";
		case CardType::Reinforcement: return os << "Reinforcement";
		case CardType::Blockade: return os << "Blockade";
		case CardType::Airlift: return os << "Airlift";
		case CardType::Diplomacy: return os << "Diplomacy";
		default: return os << "None";
	}
}

// Getter
CardType& Card::getCardType() const {
	return *cardType;
};

// Creates special order and adds it to player's list of orders
// Removes card from hand and places it back in deck
Card& Card::play(Hand& hand, Deck& deck) {
	deck.getCards().push_back(this);
	hand.getCards().erase(std::find(hand.getCards().begin(), hand.getCards().end(), this));
	return *this;
}

//////////////////////////////////////////////////
// Hand class function definitions
//////////////////////////////////////////////////

// Default constructor
Hand::Hand() 
	: cards{new std::vector<Card*>} {
	std::cout << "Called Hand constructor" << std::endl;
}

// Copy constructor
Hand::Hand(const Hand& source) 
	: cards{new std::vector<Card*>{*source.cards}} {
}

// Destructor, deallocates memory for the pointer data member and its elements
Hand::~Hand() {
	for (const Card* c : *cards) {
		delete c;
	}
	delete cards;
	std::cout << "Called Hand destructor" << std::endl;
}

// Copy assignment operator overload
Hand& Hand::operator=(const Hand& rhs) {
	if (this != &rhs) {
		for (const Card* c : *cards) {
			delete c;
		}
		delete cards;
		cards = new std::vector<Card*>{*rhs.cards};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Hand& obj) {
	os << "Hand's cards:" << std::endl;
	for (const Card* c : obj.getCards()) {
		std::cout << *c << std::endl;
	}
	return os;
}

// Getter
std::vector<Card*>& Hand::getCards() const {
	return *cards;
}

//////////////////////////////////////////////////
// Deck class function definitions
//////////////////////////////////////////////////

// Default constructor
Deck::Deck() 
	: cards{new std::vector<Card*>} {
	for (size_t i = 0; i < 5; i++) {
		cards->emplace_back(new Card{CardType::Bomb});
		cards->emplace_back(new Card{CardType::Reinforcement});
		cards->emplace_back(new Card{CardType::Blockade});
		cards->emplace_back(new Card{CardType::Airlift});
		cards->emplace_back(new Card{CardType::Diplomacy});
	}
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(cards->begin(), cards->end());
	std::cout << "Called Deck constructor" << std::endl;
}

// Copy constructor
Deck::Deck(const Deck& source) 
	: cards{new std::vector<Card*>{*source.cards}} {
}

// Destructor, deallocates memory for the pointer data member and its elements
Deck::~Deck() {
	for (const Card* c : *cards) {
		delete c;
	}
	delete cards;
	std::cout << "Called Deck destructor" << std::endl;
}

// Copy assignment operator overload
Deck& Deck::operator=(const Deck& rhs) {
	if (this != &rhs) {
		for (const Card* c : *cards) {
			delete c;
		}
		delete cards;
		cards = new std::vector<Card*>{*rhs.cards};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Deck& obj) {
	os << "Deck's cards:" << std::endl;
	for (const Card* c : obj.getCards()) {
		std::cout << *c << std::endl;
	}
	return os;
}

// Getter
std::vector<Card*>& Deck::getCards() const {
	return *cards;
}

// Removes card from deck and adds it to a player's hand
void Deck::draw(Hand& Hand) {
	Hand.getCards().push_back(*cards->begin());
	cards->erase(cards->begin());
}