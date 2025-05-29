#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Player::Player() 
	: playerName{new std::string}, reinforcementPool{new int}, playerTerritories {new std::unordered_set<const Territory*>}, playerHand{new Hand}, playerOrdersList{new OrdersList} {
	//std::cout << "Called Player default constructor" << std::endl;
}

// Parameterized constructor, only the playerName gets a value, the rest only get allocated memory
Player::Player(std::string playerName) 
	: playerName{new std::string{playerName}}, reinforcementPool{new int}, playerTerritories {new std::unordered_set<const Territory*>}, playerHand{new Hand}, playerOrdersList{new OrdersList} {
	//std::cout << "Called Player parameterized constructor" << std::endl;
}

// Copy constructor
Player::Player(const Player& source) 
	: playerName{new std::string{*source.playerName}}, reinforcementPool{new int{*source.reinforcementPool}}, 
	playerTerritories{new std::unordered_set<const Territory*>{*source.playerTerritories}}, playerHand{new Hand{*source.playerHand}}, 
	playerOrdersList{new OrdersList{*source.playerOrdersList}} {
	//std::cout << "Called Player copy constructor" << std::endl;
}

// Destructor, deallocate memory for all the pointer data members
Player::~Player() {
	delete playerName;
	delete reinforcementPool;
	delete playerTerritories;
	delete playerHand;
	delete playerOrdersList;
	//std::cout << "Called Player destructor" << std::endl;
}

// Copy assignment operator
Player& Player::operator=(const Player& rhs) {
	if (this != &rhs) {
		delete playerName;
		delete reinforcementPool;
		delete playerTerritories;
		delete playerHand;
		delete playerOrdersList;
		playerName = new std::string{*rhs.playerName};
		reinforcementPool = new int{*rhs.reinforcementPool};
		playerTerritories = new std::unordered_set<const Territory*>{*rhs.playerTerritories};
		playerHand = new Hand{*rhs.playerHand};
		playerOrdersList = new OrdersList{*rhs.playerOrdersList};
	}
	return *this;
}

// Equality operator
bool Player::operator==(const Player& rhs) const {
	return this->playerName == rhs.playerName;
}

// Getters
std::string& Player::getPlayerName() const {
	return *playerName;
}

int& Player::getReinforcementPool() const {
	return *reinforcementPool;
}

std::unordered_set<const Territory*>& Player::getPlayerTerritories() const {
	return *playerTerritories;
}

Hand& Player::getPlayerHand() const {
	return *playerHand;
}

OrdersList& Player::getPlayerOrdersList() const {
	return *playerOrdersList;
}

// For now, returns an arbitrary list of pointers of territories to defend
std::vector<Territory*> Player::toDefend() {
	std::vector<Territory*> territoriesToDefend;
	territoriesToDefend.emplace_back(new Territory{"qwer"});
	territoriesToDefend.emplace_back(new Territory{"asdf"});
	territoriesToDefend.emplace_back(new Territory{"loopas"});
	return territoriesToDefend;
}

// For now, returns an arbitrary list of pointers of territories to attack
std::vector<Territory*> Player::toAttack() {
	std::vector<Territory*> territoriesToAttack;
	territoriesToAttack.emplace_back(new Territory{"kekkles"});
	territoriesToAttack.emplace_back(new Territory{"lesasdz"});
	territoriesToAttack.emplace_back(new Territory{"polasdmk"});
	return territoriesToAttack;
}

// Adds order to player's list of orders (will probably have a parameter later)
void Player::issueOrder() {
	playerOrdersList->addOrder();
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& obj) {
	os << "[Player]" << std::endl;
	os << "Name: " << obj.getPlayerName() << std::endl;
	os << "Reinforcement Pool: " << obj.getReinforcementPool() << std::endl;
	os << "Owned Territories (" << obj.getPlayerTerritories().size() << "):" << std::endl;
	for (const Territory* t : obj.getPlayerTerritories()) {
		os << *t << std::endl;
	}
	os << "Player's Hand: " << std::endl;
	for (const Card* c : obj.getPlayerHand().getCards()) {
		os << *c << std::endl;
	}
	os << "Orders List:" << std::endl;
	for (const Order* o : obj.getPlayerOrdersList().getOrders()) {
		os << *o << std::endl;
	}
	return os;
}
