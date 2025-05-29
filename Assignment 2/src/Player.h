#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

class Territory;
class Hand;
class OrdersList;

/*
* Player class
* Keeps track of player orders, territories and hands
*/ 
class Player {
private:
	std::string* playerName;
	int* reinforcementPool;
	std::unordered_set<const Territory*>* playerTerritories;
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

	// Getters //
	std::string& getPlayerName() const;
	int& getReinforcementPool() const;
	std::unordered_set<const Territory*>& getPlayerTerritories() const;
	Hand& getPlayerHand() const;
	OrdersList& getPlayerOrdersList() const;

	// Returns list of pointers of territories to defend
	std::vector<Territory*> toDefend();
	// Returns list of pointers of territories to attack
	std::vector<Territory*> toAttack();
	// Adds order to player's list of orders
	void issueOrder();
};

#endif

