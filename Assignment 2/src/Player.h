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

/*
* Player class
* Keeps track of player orders, territories and hands
*/ 
class Player {
private:
	std::string* playerName;
	int* reinforcementPool;
	std::unordered_map<std::string, Territory*>* playerTerritories;
	Hand* playerHand;
	OrdersList* playerOrdersList;
public:
	//--Constructors--//
	// Default constructor
	Player();
	// parameterized constructor
	Player(std::string playerName);
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
	std::string& getPlayerName();
	int& getReinforcementPool();
	std::unordered_map<std::string, Territory*>& getPlayerTerritories();
	Hand& getPlayerHand();
	OrdersList& getPlayerOrdersList();
	const std::string& getPlayerName() const;
	const int& getReinforcementPool() const;
	const std::unordered_map<std::string, Territory*>& getPlayerTerritories() const;
	const Hand& getPlayerHand() const;
	const OrdersList& getPlayerOrdersList() const;

	// Returns list of pointers of territories to attack
	std::unordered_set<std::string> toAttack(CommandProcessor& cmdProcessor);
	// Returns list of pointers of territories to defend
	std::unordered_set<std::string> toDefend(CommandProcessor& cmdProcessor);
	// Adds order to player's list of orders
	void issueOrder(CommandProcessor& cmdProcessor);
};

#endif

