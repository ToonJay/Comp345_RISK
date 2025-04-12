#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include <iostream>

void testLoadMaps();
void testPlayers();
void testOrdersList();
void testCards();
void testGameStates();

int main() {
	testLoadMaps();
	std::cout << "///////////////////////////////////////////////////////" << std::endl << std::endl;
	testPlayers();
	std::cout << "\n///////////////////////////////////////////////////////" << std::endl << std::endl;
	testOrdersList();
	std::cout << "\n///////////////////////////////////////////////////////" << std::endl << std::endl;
	testCards();
	std::cout << "\n///////////////////////////////////////////////////////" << std::endl << std::endl;
	//testGameStates();

	return 0;
}