#include "Player.h"

//////////////////////////////////////////////////
// Player class function definitions
//////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Player::Player() 
	: playerName{new std::string}, playerTerritories{new std::vector<Territory*>}, playerHand{new Hand}, playerOrders{new OrdersList} {
}

// Parameterized constructor, only the playerName gets a value, the rest only get allocated memory
Player::Player(std::string playerName) 
	: playerName{new std::string{playerName}}, playerTerritories {new std::vector<Territory*>}, playerHand{new Hand}, playerOrders{new OrdersList} {
}

// Copy constructor
Player::Player(const Player& source) 
	: playerName{new std::string{*source.playerName}}, playerTerritories{new std::vector<Territory*>{*source.playerTerritories}},
	playerHand{new Hand{*source.playerHand}}, playerOrders{new OrdersList{*source.playerOrders}} {
}

// Destructor, deallocate memory for all the pointer data members
Player::~Player() {
	delete playerName;
	delete playerTerritories;
	delete playerHand;
	delete playerOrders;
}

// Copy assignment operator
Player& Player::operator=(const Player& rhs) {
	if (this != &rhs) {
		*playerName = *rhs.playerName;
		*playerTerritories = *rhs.playerTerritories;
		*playerHand = *rhs.playerHand;
		*playerOrders = *rhs.playerOrders;
	}
	return *this;
}

// Equality operator
bool Player::operator==(const Player& rhs) const {
	return this->playerName == rhs.playerName;
}

// Getters
std::string& Player::GetPlayerName() const {
	return *playerName;
}

std::vector<Territory*>& Player::GetPlayerTerritories() const {
	return *playerTerritories;
}

Hand& Player::GetPlayerHand() const {
	return *playerHand;
}

OrdersList& Player::GetPlayerOrders() const {
	return *playerOrders;
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
	playerOrders->GetOrders().emplace_back(new Blockade{});
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& obj) {
	os << "[Player]" << std::endl;
	os << "Name: " << *obj.playerName << std::endl;
	os << "Owned Territories:" << std::endl;
	for (const Territory* t : *obj.playerTerritories) {
		os << *t << std::endl;
	}
	os << "Orders List:" << std::endl;
	for (const Order* o : obj.playerOrders->GetOrders()) {
		os << *o << std::endl;
	}
	return os;
}
