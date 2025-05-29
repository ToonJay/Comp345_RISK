#ifndef _CARDS_H_
#define _CARDS_H_
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

enum class CardType {Bomb, Reinforcement, Blockade, Airlift, Diplomacy};

class Hand;
class Deck;
/*
* Card class
* Create special orders when played and adds them to the player's list of orders
*/
class Card {
	CardType* cardType;
public:
	//--Constructors--//
	// Parameterized constructor
	Card(CardType cardType);
	// Copy constructor
	Card(const Card& source);
	// Destructor
	~Card();

	//--Operator overloads--//
	// Copy assignment operator overload
	Card& operator=(const Card& rhs);
	// Equality operator overload
	bool operator==(const Card& rhs) const;
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Card& obj);

	// Getter //
	CardType& getCardType() const;

	// Creates special order and adds it to player's list of orders
	Card& play(Hand& Hand , Deck& deck);
};

/*
* Contains a list of cards that the player owns
* When a card is played from a hand, it gets returned to the deck the card was drawn from
*/
class Hand {
private:
	std::vector<Card*>* cards;
public:
	//--Constructors--//
	// Default constructor
	Hand();
	// Copy constructor
	Hand(const Hand& source);
	// Destructor
	~Hand();

	//--Operator overloads--//
	// Copy assignment operator overload
	Hand& operator=(const Hand& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Hand& obj);

	// Getter //
	std::vector<Card*>& getCards() const;
};

/*
* Contains a finite amount of cards of all types
* A player can draw from the deck, which removes the card from the list
* When the card is played, it gets added back into the deck
*/
class Deck {
private:
	std::vector<Card*>* cards;
public:
	//--Constructors--//
	// Default constructor
	Deck();
	// Copy constructor
	Deck(const Deck& source);
	// Destructor
	~Deck();

	//--Operator overloads--//
	// Copy assignment operator overload
	Deck& operator=(const Deck& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Deck& obj);

	// Getter //
	std::vector<Card*>& getCards() const;

	// Removes card from deck and adds it to a player's hand
	void draw(Hand& hand);
};

#endif