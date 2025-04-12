#include "Player.h"

void testPlayers() {
	Territory* t1{new Territory{"Peterland"}};
	Territory* t2{new Territory{"Peterland2"}};
	Territory* t3{new Territory{"Peterland3"}};
	Player player{"Jason"};
	player.GetPlayerTerritories().push_back(t1);
	player.GetPlayerTerritories().push_back(t2);
	player.GetPlayerTerritories().push_back(t3);
	player.issueOrder();
	player.issueOrder();
	player.issueOrder();
	player.GetPlayerOrders().GetOrders().at(0)->execute();
	player.GetPlayerOrders().GetOrders().at(1)->execute();
	player.GetPlayerOrders().GetOrders().at(2)->execute();
	player.GetPlayerHand().GetCards().emplace_back(new Card(CardType::Bomb));
	player.GetPlayerHand().GetCards().emplace_back(new Card(CardType::Blockade));
	player.GetPlayerHand().GetCards().emplace_back(new Card(CardType::Airlift));
	std::cout << player << std::endl;
	delete t1;
	delete t2;
	delete t3;
	
	std::vector<Territory*> attackVector{player.toAttack()};
	std::vector<Territory*> defendVector{player.toDefend()};
	for (const Territory* t : attackVector) {
		std::cout << *t << std::endl;
	}
	for (const Territory* t : defendVector) {
		std::cout << *t << std::endl;
	}
	for (const Territory* t : attackVector) {
		delete t;
	}
	for (const Territory* t : defendVector) {
		delete t;
	}
}