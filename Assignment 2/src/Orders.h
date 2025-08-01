#ifndef _ORDERS_H_
#define _ORDERS_H_
#include <vector>
#include <iostream>
#include "LoggingObserver.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE : I'm skipping the << and = overloads for Order's Subclasses since they're completely unneeded.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GameEngine;
class Player;

/*
* Order class
* Will be the base class for all types of orders
* Making this an abstract class since I'll only use the subclasses
*/
class Order : public Subject, public ILoggable {
private:
	std::string* orderDescription;
protected:
	std::string* orderEffect;
public:
	//--Constructors--//
	// Parameterized constructor
	Order(std::string orderDescription);
	// Copy constructor
	Order(const Order& source);
	// Destructor
	virtual ~Order();

	// Pure virtual clone
	virtual Order* clone() const = 0; 

	//--Operator overloads--//
	// Copy assignment operator overload
	Order& operator=(const Order& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Order& obj);

	//--Getters--//
	const std::string& getOrderDescription() const;

	// Verifies whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const = 0;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) = 0;

	// Logs order's effect
	virtual std::string stringToLog() const override;
};

/*
* Order subclasses for each type of Order
* Will be the base class for all types of orders
*/
class Deploy : public Order {
private:
	int* numOfUnits;
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Deploy(int numOfUnits, std::string target);
	// Copy constructor
	Deploy(const Deploy& source);
	// Destructor
	virtual ~Deploy();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

class Advance : public Order {
private:
	int* numOfUnits;
	std::string* source;
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Advance(int numOfUnits, std::string source, std::string target);
	// Copy constructor
	Advance(const Advance& source);
	// Destructor
	virtual ~Advance();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

class Bomb : public Order {
private:
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Bomb(std::string target);
	// Copy constructor
	Bomb(const Bomb& source);
	// Destructor
	virtual ~Bomb();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

class Blockade : public Order {
private:
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Blockade(std::string target);
	// Copy constructor
	Blockade(const Blockade& source);
	// Destructor
	virtual ~Blockade();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

class Airlift : public Order {
private:
	int* numOfUnits;
	std::string* source;
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Airlift(int numOfUnits, std::string source, std::string target);
	// Copy constructor
	Airlift(const Airlift& source);
	// Destructor
	virtual ~Airlift();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

class Negotiate : public Order {
private:
	std::string* target;
public:
	//--Constructors--//
	// Parameterized constructor
	Negotiate(std::string target);
	// Copy constructor
	Negotiate(const Negotiate& source);
	// Destructor
	virtual ~Negotiate();

	// Returns a copy of the object
	virtual Order* clone() const override;

	// Method to validate whether or not the order can be executed
	virtual bool validate(GameEngine& game, Player& player) const override;
	// Execute a valid order
	virtual void execute(GameEngine& game, Player& player) override;
};

/*
* OrdersList class
* Contains a Player's list of orders
*/
class OrdersList : public Subject, public ILoggable {
private:
	std::vector<Order*>* orders;
	LogObserver* logObserver;
public:
	//--Constructors--//
	// Default constructor
	OrdersList();
	// Copy constructor
	OrdersList(const OrdersList& source);
	// Destructor
	~OrdersList();

	//--Operator overloads--//
	// Copy assignment operator overload
	OrdersList& operator=(const OrdersList& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const OrdersList& obj);

	//--Getters--//
	LogObserver& getLogObserver();
	const std::vector<Order*>& getOrders() const;
	const LogObserver& getLogObserver() const;

	//--Mutators--//
	// Add order to player's OrdersList
	void addOrder(Order* order);
	// Remove order from orders
	void remove(int index);
	// Moves an order to a specific index of orders
	void move(int source, int destination);
	// Set the LogObserver pointer
	void setLogObserver(LogObserver* logObserver);

	// Logs order description of the last order
	virtual std::string stringToLog() const override;
};

#endif