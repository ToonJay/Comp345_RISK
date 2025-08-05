#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class Territory;
class Hand;
class OrdersList;
class CommandProcessor;
class Map;
class Deck;
class LogObserver;

/*
* Player class
* Keeps track of player orders, territories and hands
*/
class Player {
private:
	std::string* playerName;
	int* reinforcementPool;
	std::unordered_set<std::string>* playerTerritories;
	Hand* playerHand;
	OrdersList* playerOrdersList;
	bool* drawCard;
public:
	//--Constructors--//
	// Default constructor
	Player();
	// parameterized constructor
	Player(std::string playerName, LogObserver* logObserver);
	// Copy constructor
	Player(const Player& source);
	// Destructor constructor
	~Player();

	//--Operator overloads--//
	// Copy assignment operator overload
	Player& operator=(const Player& rhs);
	// Equality operator overload
	bool operator==(const Player& rhs) const;
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);

	//--Getters--//
	Hand& getPlayerHand();
	OrdersList& getPlayerOrdersList();
	bool& getDrawCard();
	const std::string& getPlayerName() const;
	const int& getReinforcementPool() const;
	const std::unordered_set<std::string>& getPlayerTerritories() const;
	const Hand& getPlayerHand() const;
	const OrdersList& getPlayerOrdersList() const;
	const bool& getDrawCard() const;

	//--Mutators--//
	void setReinforcementPool(int reinforcementPool);
	void addReinforcements(int numOfUnits);
	void sendReinforcements(int numOfUnits);
	void addTerritory(const std::string& territory);
	void removeTerritory(const std::string& territory);

	// Returns list of pointers of territories to attack
	std::unordered_set<std::string> toAttack(CommandProcessor& cmdProcessor);
	// Returns list of pointers of territories to defend
	std::unordered_set<std::string> toDefend(CommandProcessor& cmdProcessor);
	// Adds order to player's list of orders
	void issueOrder(CommandProcessor& cmdProcessor, Deck& deck);
};

#endif