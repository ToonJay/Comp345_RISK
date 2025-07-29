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
	: playerName{new std::string}, reinforcementPool{new int}, playerTerritories {new std::unordered_map<std::string, Territory*>}, playerHand{new Hand}, 
	playerOrdersList{new OrdersList}, drawCard{new bool{false}} {
	//std::cout << "Called Player default constructor" << std::endl;
}

// Parameterized constructor, only the playerName gets a value, the rest only get allocated memory
Player::Player(std::string playerName) 
	: playerName{new std::string{playerName}}, reinforcementPool{new int}, playerTerritories {new std::unordered_map<std::string, Territory*>}, playerHand{new Hand}, 
	playerOrdersList{new OrdersList}, drawCard{new bool{false}} {
	//std::cout << "Called Player parameterized constructor" << std::endl;
}

// Copy constructor
Player::Player(const Player& source) 
	: playerName{new std::string{*source.playerName}}, reinforcementPool{new int{*source.reinforcementPool}}, 
	playerTerritories{new std::unordered_map<std::string, Territory*>{*source.playerTerritories}}, playerHand{new Hand{*source.playerHand}}, 
	playerOrdersList{new OrdersList{*source.playerOrdersList}}, drawCard{new bool{*source.drawCard}} {
	//std::cout << "Called Player copy constructor" << std::endl;
}

// Destructor, deallocate memory for all the pointer data members
Player::~Player() {
	delete playerName;
	delete reinforcementPool;
	delete playerTerritories;
	delete playerHand;
	delete playerOrdersList;
	delete drawCard;
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
		delete drawCard;
		playerName = new std::string{*rhs.playerName};
		reinforcementPool = new int{*rhs.reinforcementPool};
		playerTerritories = new std::unordered_map<std::string, Territory*>{*rhs.playerTerritories};
		playerHand = new Hand{*rhs.playerHand};
		playerOrdersList = new OrdersList{*rhs.playerOrdersList};
		drawCard = new bool{*rhs.drawCard};
	}
	return *this;
}

// Equality operator
bool Player::operator==(const Player& rhs) const {
	return this->playerName == rhs.playerName;
}

// Getters
Hand& Player::getPlayerHand() {
	return *playerHand;
}

OrdersList& Player::getPlayerOrdersList() {
	return *playerOrdersList;
}

bool& Player::getDrawCard() {
	return *drawCard;
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

const bool& Player::getDrawCard() const {
	return *drawCard;
}

// Mutators
void Player::setReinforcementPool(int reinforcementPool) {
	*this->reinforcementPool = (reinforcementPool >= 0) ? reinforcementPool : 0;
}

void Player::addReinforcements(int numOfUnits) {
	*this->reinforcementPool += numOfUnits;
}

void Player::sendReinforcements(int numOfUnits) {
	*this->reinforcementPool -= numOfUnits;
	if (*this->reinforcementPool < 0) {
		*this->reinforcementPool = 0;
	}
}

void Player::addTerritory(Territory* const territory) {
	(*playerTerritories)[territory->getName()] = territory;
}

void Player::removeTerritory(Territory* const territory) {
	(*playerTerritories).erase(territory->getName());
}

// For now, returns an arbitrary list of pointers of territories to attack
std::unordered_set<std::string> Player::toAttack(CommandProcessor& cmdProcessor) {
	std::cout << "First, which territories would you like to attack?" << std::endl;
	std::unordered_set<std::string> attackList;
	std::string cmdString{""};

	while (cmdString != "continue") {
		std::cout << "\nEnter a territory name to add to your attack list or \"continue\" when you're done." << std::endl;
		cmdString.clear();
		std::istringstream iss{cmdProcessor.getCommand().getCommand()};
		std::getline(iss, cmdString);
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

	while (cmdString != "continue" || defendList.size() == 0) {
		std::cout << "\nEnter a territory name to add to your defend list or \"continue\" when you're done." << std::endl;
		cmdString.clear();
		std::istringstream iss{cmdProcessor.getCommand().getCommand()};
		std::getline(iss, cmdString);
		if (cmdString != "continue") {
			if (defendList.find(cmdString) != defendList.end()) {
				std::cout << "That territory is already in your defend list." << std::endl;
			} else {
				defendList.insert(cmdString);
			}
		} else if (cmdString == "continue" && defendList.size() == 0) {
			std::cout << "You need at least one territory in your defend list to be able to deploy all your units." << std::endl;
		}
	}
	return defendList;
}

// Adds order to player's list of orders (will probably have a parameter later)
void Player::issueOrder(CommandProcessor& cmdProcessor, Deck& deck) {
	std::cout << std::endl << *playerName << "'s turn to issue orders." << std::endl;
	std::unordered_set<std::string> attackList;
	attackList = toAttack(cmdProcessor);
	std::unordered_set<std::string> defendList;
	defendList = toDefend(cmdProcessor);
	std::string cmdString{""};

	while (cmdString != "done" || *reinforcementPool > 0) {
		std::cout << "Your attack list:" << std::endl;
		for (const std::string& t : attackList) {
			std::cout << t << std::endl;
		}
		std::cout << "Your defend list:" << std::endl;
		for (const std::string& t : defendList) {
			std::cout << t << std::endl;
		}

		std::cout << "\nWhat type of order do you want to issue?" << std::endl;
		cmdString.clear();
		std::istringstream iss{cmdProcessor.getCommand().getCommand()};
		iss >> cmdString;
		iss >> std::ws;

		if (*reinforcementPool > 0) {
			if (cmdString == "deploy") {
				int numOfUnits{};
				std::string target{};
				iss >> numOfUnits;
				iss >> std::ws;
				std::getline(iss, target);

				if (defendList.find(target) != defendList.end()) {
					if (*reinforcementPool >= numOfUnits) {
						*reinforcementPool -= numOfUnits;
					} else {
						numOfUnits = *reinforcementPool;
						std::cout << "Only " << std::to_string(*reinforcementPool) << " units were left in your reinforcements." << std::endl;
						*reinforcementPool = 0;
					}
					getPlayerOrdersList().addOrder(new Deploy{numOfUnits, target});
				} else {
					std::cout << "That Territory isn't in your defend list."  << std::endl;
				}
			} else {
				std::cout << "You still have " << *reinforcementPool << " left to deploy." << std::endl;
			} 
		} else if (cmdString == "deploy") {
			std::cout << "You have no more units to deploy." << std::endl;
		} else if (cmdString == "advance") {
			if (attackList.size() + defendList.size() < 2) {
				std::cout << "You only have 1 Territory on your lists combined, you can't make an advance order." << std::endl;
				continue;
			}

			int numOfUnits{};
			std::string source{};
			std::string target{};
			iss >> numOfUnits;
			iss >> std::ws;
			std::getline(iss, source, ':');
			std::getline(iss, target);

			if (defendList.find(source) == defendList.end()) {
				std::cout << "The source Territory isn't in your defend list." << std::endl;
				continue;
			} else if (defendList.find(target) == defendList.end() && attackList.find(target) == attackList.end()) {
				std::cout << "The target Territory isn't in either list." << std::endl;
				continue;
			} else if (source == target) {
				std::cout << "The source and target cannot be the same territory." << std::endl;
				continue;
			}

			getPlayerOrdersList().addOrder(new Advance(numOfUnits, source, target));
		} else if (cmdString == "airlift") {
			if (defendList.size() < 2) {
				std::cout << "You only have 1 Territory in your defend list, you can't make an airlift order." << std::endl;
				continue;
			}

			auto it = std::find_if(getPlayerHand().getCards().begin(), getPlayerHand().getCards().end(),
				[](const Card* c) { return c->getCardType() == CardType::Airlift; });
			if (it == getPlayerHand().getCards().end()) {
				std::cout << "You don't have an airlift card, you need one to make an airlift order." << std::endl;
				continue;
			} 

			int numOfUnits{};
			std::string source{};
			std::string target{};
			iss >> numOfUnits;
			iss >> std::ws;
			std::getline(iss, source, ':');
			std::getline(iss, target);

			if (defendList.find(source) == defendList.end()) {
				std::cout << "The source Territory isn't in your defend list." << std::endl;
				continue;
			} else if (defendList.find(target) == defendList.end()) {
				std::cout << "The target Territory isn't in your defend list." << std::endl;
				continue;
			} else if (source == target) {
				std::cout << "The source and target cannot be the same territory." << std::endl;
				continue;
			}

			(*it)->play(getPlayerHand(), deck);
			getPlayerOrdersList().addOrder(new Airlift(numOfUnits, source, target));
		} else if (cmdString == "bomb") {
			if (attackList.size() < 1) {
				std::cout << "Your attack list is empty, there's no one to bomb." << std::endl;
				continue;
			}

			auto it = std::find_if(getPlayerHand().getCards().begin(), getPlayerHand().getCards().end(),
				[](const Card* c) { return c->getCardType() == CardType::Bomb; });
			if (it == getPlayerHand().getCards().end()) {
				std::cout << "You don't have a bomb card, you need one to make a bomb order." << std::endl;
				continue;
			}

			std::string target{};
			std::getline(iss, target);

			if (attackList.find(target) == attackList.end()) {
				std::cout << "The target Territory isn't in your attack list." << std::endl;
				continue;
			} 

			(*it)->play(getPlayerHand(), deck);
			getPlayerOrdersList().addOrder(new Bomb(target));
		} else if (cmdString == "blockade") {
			auto it = std::find_if(getPlayerHand().getCards().begin(), getPlayerHand().getCards().end(),
				[](const Card* c) { return c->getCardType() == CardType::Blockade; });
			if (it == getPlayerHand().getCards().end()) {
				std::cout << "You don't have a blockade card, you need one to make a blockade order." << std::endl;
				continue;
			}

			std::string target{};
			std::getline(iss, target);

			if (defendList.find(target) == defendList.end()) {
				std::cout << "The target Territory isn't in your defend list." << std::endl;
				continue;
			}

			(*it)->play(getPlayerHand(), deck);
			getPlayerOrdersList().addOrder(new Blockade(target));
		} else if (cmdString == "negotiate") {
			auto it = std::find_if(getPlayerHand().getCards().begin(), getPlayerHand().getCards().end(),
				[](const Card* c) { return c->getCardType() == CardType::Diplomacy; });
			if (it == getPlayerHand().getCards().end()) {
				std::cout << "You don't have a diplomacy card, you need one to make a negotiate order." << std::endl;
				continue;
			}

			std::string player{};
			std::getline(iss, player);
			std::cout << player << std::endl;
			(*it)->play(getPlayerHand(), deck);
			getPlayerOrdersList().addOrder(new Negotiate(player));
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