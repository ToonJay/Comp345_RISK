#include "Cards.h"

void testCards() {
	Deck deck{};
	Hand hand{};
	std::cout << deck;

	for (size_t i = 0; i < 10; i++) {
		deck.draw(hand);
	}
	
	std::cout << "\n***After 10 draws***\n" << std::endl;
	std::cout << hand << std::endl;
	std::cout << deck << std::endl;

	std::cout << "\n***Play 10 cards from hand***\n" << std::endl;
	for (size_t i = 0; i < 10; i++) {
		std::cout << hand.getCards().at(10 - 1 - i)->play(hand, deck) << std::endl;
	}
	
	std::cout << "\n***After 10 plays***\n" << std::endl;
	std::cout << hand << std::endl;
	std::cout << deck << std::endl;
}