#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Map.h"
#include "Cards.h"
#include "Orders.h"
#include <iostream>
#include <string>
#include <vector>
/*
* Player class
* Keeps track of player orders, territories and hands
*/ 
class Player {
private:
	std::string* playerName;
	std::vector<Territory*>* playerTerritories;
	Hand* playerHand;
	OrdersList* playerOrders;
public:
	// Constructors
	// Default constructor
	Player();
	// Parametized constructor
	Player(std::string playerName);
	// Copy constructor
	Player(const Player& source);
	// Destructor constructor
	~Player();

	// Operator overloads
	// Copy assignment operator overload
	Player& operator=(const Player& rhs);
	// Equality operator overload
	bool operator==(const Player& rhs) const;
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Player& obj);

	// Getters
	std::string& GetPlayerName() const;
	std::vector<Territory*>& GetPlayerTerritories() const;
	Hand& GetPlayerHand() const;
	OrdersList& GetPlayerOrders() const;

	// Returns list of pointers of territories to defend
	std::vector<Territory*> toDefend();
	// Returns list of pointers of territories to attack
	std::vector<Territory*> toAttack();
	// Adds order to player's list of orders
	void issueOrder();
};

#endif

