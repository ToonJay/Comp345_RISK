#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "Orders.h"
#include "CommandProcessing.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Player class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor, allocates memory, but doesn't assign values
Player::Player() 
	: playerName{new std::string}, reinforcementPool{new int}, playerTerritories {new std::unordered_map<std::string, Territory*>}, playerHand{new Hand}, playerOrdersList{new OrdersList} {
	//std::cout << "Called Player default constructor" << std::endl;
}

// Parameterized constructor, only the playerName gets a value, the rest only get allocated memory
Player::Player(std::string playerName) 
	: playerName{new std::string{playerName}}, reinforcementPool{new int}, playerTerritories {new std::unordered_map<std::string, Territory*>}, playerHand{new Hand}, playerOrdersList{new OrdersList} {
	//std::cout << "Called Player parameterized constructor" << std::endl;
}

// Copy constructor
Player::Player(const Player& source) 
	: playerName{new std::string{*source.playerName}}, reinforcementPool{new int{*source.reinforcementPool}}, 
	playerTerritories{new std::unordered_map<std::string, Territory*>{*source.playerTerritories}}, playerHand{new Hand{*source.playerHand}}, 
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
		playerTerritories = new std::unordered_map<std::string, Territory*>{*rhs.playerTerritories};
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
std::string& Player::getPlayerName() {
	return *playerName;
}

int& Player::getReinforcementPool() {
	return *reinforcementPool;
}

std::unordered_map<std::string, Territory*>& Player::getPlayerTerritories() {
	return *playerTerritories;
}

Hand& Player::getPlayerHand() {
	return *playerHand;
}

OrdersList& Player::getPlayerOrdersList() {
	return *playerOrdersList;
}

const std::string& Player::getPlayerName() const {
	return *playerName;
}

const int& Player::getReinforcementPool() const {
	return *reinforcementPool;
}

const std::unordered_map<std::string, Territory*>& Player::getPlayerTerritories() const {
	return *playerTerritories;
}

const Hand& Player::getPlayerHand() const {
	return *playerHand;
}

const OrdersList& Player::getPlayerOrdersList() const {
	return *playerOrdersList;
}

// For now, returns an arbitrary list of pointers of territories to attack
std::unordered_set<std::string> Player::toAttack(CommandProcessor& cmdProcessor) {
	std::cout << "First, which territories would you like to attack?" << std::endl;
	std::unordered_set<std::string> attackList;
	std::string cmdString{""};

	while (cmdString != "continue") {
		std::cout << "\nEnter a territory name to add to your attack list or \"continue\" when you're done." << std::endl;
		cmdString.clear();
		cmdProcessor.getCommand();
		std::istringstream iss{cmdProcessor.getCommandList().back().getCommand()};
		iss >> cmdString;
		if (cmdString != "continue") {

			if (attackList.find(cmdString) != attackList.end()) {
				std::cout << "That territory is already in your attack list." << std::endl;
			} else {
				attackList.insert(cmdString);
			}
		}
	}
	return attackList;
}

// For now, returns an arbitrary list of pointers of territories to defend
std::unordered_set<std::string> Player::toDefend(CommandProcessor& cmdProcessor) {
	std::cout << "\nNext, which territories would you like to defend?" << std::endl;
	std::unordered_set<std::string> defendList;
	std::string cmdString{""};

	while (cmdString != "continue") {
		std::cout << "\nEnter a territory name to add to your defend list or \"continue\" when you're done." << std::endl;
		cmdString.clear();
		cmdProcessor.getCommand();
		std::istringstream iss{cmdProcessor.getCommandList().back().getCommand()};
		iss >> cmdString;
		if (cmdString != "continue") {
			if (defendList.find(cmdString) != defendList.end()) {
				std::cout << "That territory is already in your defend list." << std::endl;
			} else {
				defendList.insert(cmdString);
			} 
		}
	}
	return defendList;
}

// Adds order to player's list of orders (will probably have a parameter later)
void Player::issueOrder(CommandProcessor& cmdProcessor) {
	std::cout << std::endl << *playerName << "'s turn to issue orders." << std::endl;
	std::unordered_set<std::string> attackList;
	attackList = toAttack(cmdProcessor);
	std::unordered_set<std::string> defendList;
	defendList = toDefend(cmdProcessor);
	std::string cmdString{""};

	std::cout << "Your attack list:" << std::endl;
	for (const std::string& t : attackList) {
		std::cout << t << std::endl;
	}

	std::cout << "Your defend list:" << std::endl;
	for (const std::string& t : defendList) {
		std::cout << t << std::endl;
	}

	while (cmdString != "endissueorders") {
		std::cout << "\nWhat type of order do you want to issue?" << std::endl;
		cmdString.clear();
		cmdProcessor.getCommand();
		std::istringstream iss{cmdProcessor.getCommandList().back().getCommand()};
		iss >> cmdString;
		if (*reinforcementPool > 0 && cmdString != "deploy") {
			std::cout << "You have " << *reinforcementPool << " left to deploy." << std::endl;
		} else {
			playerOrdersList->addOrder();
		}
	}
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& obj) {
	os << "[Player]" << std::endl;
	os << "Name: " << obj.getPlayerName() << std::endl;
	os << "Reinforcement Pool: " << obj.getReinforcementPool() << std::endl;
	os << "Owned Territories (" << obj.getPlayerTerritories().size() << "):" << std::endl;
	for (const auto& t : obj.getPlayerTerritories()) {
		os << *t.second << std::endl;
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