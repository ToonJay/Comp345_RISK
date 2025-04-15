#ifndef _ORDERS_H_
#define _ORDERS_H_
#include <vector>
#include <iostream>

/*
* Order class
* Will be the base class for all types of orders
* Making this an abstract class since I'll only use the subclasses
*/
class Order {
private:
	std::string* orderDescription;
protected:
	std::string* orderEffect;
public:
	// Constructors
	// Parameterized constructor
	Order(std::string orderDescription);
	// Copy constructor
	Order(const Order& source);
	// Destructor
	virtual ~Order();

	// Operator overloads
	// Copy assignment operator overload
	Order& operator=(const Order& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Order& obj);

	// Method to validate whether or not the order can be executed
	virtual bool validate() = 0;
	// Execute a valid order
	virtual void execute() = 0;
};

/*
* Order subclasses for each type of Order
* Will be the base class for all types of orders
*/
class Deploy : public Order {
public:
	// Constructors
	// Default constructor
	Deploy();
	// Copy constructor
	Deploy(const Deploy& source);
	// Destructor
	virtual ~Deploy();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

class Advance : public Order {
public:
	// Constructors
	// Default constructor
	Advance();
	// Copy constructor
	Advance(const Advance& source);
	// Destructor
	virtual ~Advance();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

class Bomb : public Order {
public:
	// Constructors
	// Default constructor
	Bomb();
	// Copy constructor
	Bomb(const Bomb& source);
	// Destructor
	virtual ~Bomb();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

class Blockade : public Order {
public:
	// Constructors
	// Default constructor
	Blockade();
	// Copy constructor
	Blockade(const Blockade& source);
	// Destructor
	virtual ~Blockade();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

class Airlift : public Order {
public:
	// Constructors
	// Default constructor
	Airlift();
	// Copy constructor
	Airlift(const Airlift& source);
	// Destructor
	virtual ~Airlift();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

class Negotiate : public Order {
public:
	// Constructors
	// Default constructor
	Negotiate();
	// Copy constructor
	Negotiate(const Negotiate& source);
	// Destructor
	virtual ~Negotiate();

	// Method to validate whether or not the order can be executed
	virtual bool validate() override;
	// Execute a valid order
	virtual void execute() override;
};

/*
* OrdersList class
* Contains a Player's list of orders
*/
class OrdersList {
private:
	std::vector<Order*>* orders;
public:
	// Constructors
	// Default constructor
	OrdersList();
	// Copy constructor
	OrdersList(const OrdersList& source);
	// Destructor
	~OrdersList();

	// Operator overloads
	// Copy assignment operator overload
	OrdersList& operator=(const OrdersList& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const OrdersList& obj);

	// Getter
	std::vector<Order*>& getOrders() const;

	// Remove order from orders
	void remove(int index);
	// Moves an order to a specific index of orders
	void move(int source, int destination);
	
};

#endif