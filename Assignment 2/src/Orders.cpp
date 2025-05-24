#include "Orders.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Order class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Order::Order(std::string orderDescription)
	: orderDescription{new std::string{orderDescription}}, orderEffect{new std::string{"No effect."}} {
	//std::cout << "Called Order parameterized constructor" << std::endl;
}

// Copy constructor
Order::Order(const Order& source) 
	: orderDescription {new std::string{*source.orderDescription}}, orderEffect{new std::string{*source.orderEffect}} {
	//std::cout << "Called Order copy constructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Order::~Order() {
	delete orderDescription;
	//std::cout << "Called Order destructor" << std::endl;
}

// Copy assignment operator overload
Order& Order::operator=(const Order& rhs) {
	if (this != &rhs) {
		delete orderDescription;
		delete orderEffect;
		orderDescription = new std::string{*rhs.orderDescription};
		orderEffect = new std::string{*rhs.orderEffect};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Order& obj) {
	os << "Description:" << std::endl;
	os << *obj.orderDescription << std::endl;
	if (obj.orderEffect->compare("No effect.") != 0) {
		os << "Effect:" << std::endl;
		os << *obj.orderEffect << std::endl;
	}
	return os;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deploy subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Deploy::Deploy()
	: Order{"Put a certain number of army units on a target territory"} {
	//std::cout << "Called Deploy default contructor" << std::endl;
}

// Copy constructor
Deploy::Deploy(const Deploy& source) 
	: Order{source} {
	//std::cout << "Called Deploy copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Deploy::~Deploy() {
	delete orderEffect;
	//std::cout << "Called Deploy destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Deploy::validate() {
	return false;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Deploy::execute() {
	if (validate()) {
		orderEffect->assign("20 army units have been put on Usbeclador");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advance subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Advance::Advance()
	: Order{"Move a certain number of army units from one territory (source territory) to another territory (target territory)"} {
	//std::cout << "Called Advance default contructor" << std::endl;
}

// Copy constructor
Advance::Advance(const Advance& source)
	: Order{source} {
	//std::cout << "Called Advance copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Advance::~Advance() {
	delete orderEffect;
	//std::cout << "Called Advance destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Advance::validate() {
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Advance::execute() {
	if (validate()) {
		orderEffect->assign("Moved 5 army units from A to B");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bomb subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Bomb::Bomb()
	: Order{"Destroy half of the army units located on a target territory.\nThis order can only be issued if a player has the bomb card in their hand."} {
	//std::cout << "Called Bomb default contructor" << std::endl;
}

// Copy constructor
Bomb::Bomb(const Bomb& source)
	: Order{source} {
	//std::cout << "Called Bomb copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Bomb::~Bomb() {
	delete orderEffect;
	//std::cout << "Called Bomb destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Bomb::validate() {
	return false;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Bomb::execute() {
	if (validate()) {
		orderEffect->assign("Destroyed half of the army units of Territory C");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Blockade subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Blockade::Blockade()
	: Order{"Triple the number of army units on a target territory and make it a neutral territory.\nThis order can only be issued if a player has the blockade card in their hand."} {
	//std::cout << "Called Blockade default contructor" << std::endl;
}

// Copy constructor
Blockade::Blockade(const Blockade& source)
	: Order{source} {
	//std::cout << "Called Blockade copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Blockade::~Blockade() {
	delete orderEffect;
	//std::cout << "Called Blockade destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Blockade::validate() {
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Blockade::execute() {
	if (validate()) {
		orderEffect->assign("Tripled the army units of Territory D and made it neutral.");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Airlift subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Airlift::Airlift()
	: Order{"Advance a certain number of army units from one from one territory (source territory) to another territory(target territory).\nThis order can only be issued if a player has the airlift card in their hand."} {
	//std::cout << "Called Airlift default contructor" << std::endl;
}

// Copy constructor
Airlift::Airlift(const Airlift& source)
	: Order{source} {
	//std::cout << "Called Airlift copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Airlift::~Airlift() {
	delete orderEffect;
	//std::cout << "Called Airlift destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Airlift::validate() {
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Airlift::execute() {
	if (validate()) {
		orderEffect->assign("Advanced 13 army units from B to F");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Negotiate subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Negotiate::Negotiate()
	: Order{"Prevent attacks between the current player and another target player until the end of the turn.\nThis order can only be issued if a player has the diplomacy card in their hand."} {
	//std::cout << "Called Negotiate default contructor" << std::endl;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& source)
	: Order{source} {
	//std::cout << "Called Negotiate copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Negotiate::~Negotiate() {
	delete orderEffect;
	//std::cout << "Called Negotiate destructor" << std::endl;
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Negotiate::validate() {
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Negotiate::execute() {
	if (validate()) {
		orderEffect->assign("Player A has negotiated a temporary truce with Player B (both players cannot attack eachother until the next turn).");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OrdersList class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
OrdersList::OrdersList() 
	: orders{new std::vector<Order*>} {
	//std::cout << "Called OrdersList default constructor" << std::endl;
}

// Copy constructor
OrdersList::OrdersList(const OrdersList& source) 
	: orders{new std::vector<Order*>{*source.orders}} {
	//std::cout << "Called OrdersList copy constructor" << std::endl;
}

// Destructor, deallocates memory for the pointer data member
OrdersList::~OrdersList() {
	for (const Order* o : *orders) {
		delete o;
	}
	delete orders;
	//std::cout << "Called OrdersList destructor" << std::endl;
}

// Copy assignment operator overload
OrdersList& OrdersList::operator=(const OrdersList& rhs) {
	if (this != &rhs) {
		for (const Order* o : *orders) {
			delete o;
		}
		delete orders;
		orders = new std::vector<Order*>{*rhs.orders};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const OrdersList& obj) {
	os << "Orders:" << std::endl;
	for (const Order* o : obj.getOrders()) {
		std::cout << *o << std::endl;
	}
	return os;
}

// Getter
std::vector<Order*>& OrdersList::getOrders() const {
	return *orders;
}

// Add order to player's OrdersList
void OrdersList::addOrder() {
	getOrders().emplace_back(new Blockade{});
}

// Remove order pointer and delete the pointed to object
void OrdersList::remove(int index) {
	if (orders->at(index)) {
		delete orders->at(index);
		orders->erase(orders->begin() + index);
	}
}

// Move Order pointer to a different index on the vector
void OrdersList::move(int source, int destination) {
	if (orders->at(source)) {
		if (orders->at(destination)) {
			Order* o{orders->at(source)};
			orders->erase(orders->begin() + source);
			orders->insert(orders->begin() + destination, o);
			
		}
	}
}