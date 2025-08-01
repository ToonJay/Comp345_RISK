#include "Orders.h"
#include "GameEngine.h"
#include "Player.h"

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
	delete orderEffect;
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

const std::string& Order::getOrderDescription() const {
	return *orderDescription;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Order& obj) {
	os << "Description:" << std::endl;
	os << *obj.orderDescription << std::endl;
	if (*obj.orderEffect != "No effect.") {
		os << "Effect:" << std::endl;
		os << *obj.orderEffect << std::endl;
	}
	return os;
}

// Logs order's effect
std::string Order::stringToLog() const {
	return *orderEffect;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deploy subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Deploy::Deploy(int numOfUnits, std::string target)
	: Order{"Put a certain number of army units on a target territory"}, numOfUnits{new int{numOfUnits}}, target{new std::string{target}} {
	//std::cout << "Called Deploy parameterized contructor" << std::endl;
}

// Copy constructor
Deploy::Deploy(const Deploy& source) 
	: Order{source}, numOfUnits{new int{*source.numOfUnits}}, target{new std::string{*source.target}} {
	//std::cout << "Called Deploy copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Deploy::~Deploy() {
	delete numOfUnits;
	delete target;
	//std::cout << "Called Deploy destructor" << std::endl;
}

// Returns a copy of the object
Order* Deploy::clone() const {
	return new Deploy(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Deploy::validate(GameEngine& game, Player& player) const {
	if (game.getMapLoader().getMap().getTerritories().find(*target) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *target + " is not an existing territory.";
		return false;
	}
	if (player.getPlayerTerritories().find(*target) == player.getPlayerTerritories().end()) {
		*orderEffect = "No effect since you don't own " + *target + ".";
		return false;
	}
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Deploy::execute(GameEngine& game, Player& player) {
	if (validate(game, player)) {
		player.getPlayerTerritories().find(*target)->second->addUnits(*numOfUnits);
		*orderEffect = std::to_string(*numOfUnits) + " army units have been deployed to " + *target + ".";
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Advance subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Advance::Advance(int numOfUnits, std::string source, std::string target)
	: Order{"Move a certain number of army units from one territory (source territory) to another (target territory)"},
	numOfUnits{new int{numOfUnits}}, source{new std::string{source}}, target{new std::string{target}} {
	//std::cout << "Called Advance parameterized contructor" << std::endl;
}

// Copy constructor
Advance::Advance(const Advance& source)
	: Order{source}, numOfUnits{new int{*source.numOfUnits}}, source{new std::string{*source.source}}, target{new std::string{*source.target}} {
	//std::cout << "Called Advance copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Advance::~Advance() {
	delete numOfUnits;
	delete source;
	delete target;
	//std::cout << "Called Advance destructor" << std::endl;
}

// Returns a copy of the object
Order* Advance::clone() const {
	return new Advance(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Advance::validate(GameEngine& game, Player& player) const {
	if (game.getMapLoader().getMap().getTerritories().find(*source) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *source + " is not an existing territory.";
		return false;
	}
	if (game.getMapLoader().getMap().getTerritories().find(*target) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *target + " is not an existing territory.";
		return false;
	}
	Territory& sourceTerritory{game.getMapLoader().getMap().getTerritoryByName(*source)};
	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
	std::string sourceOwner{sourceTerritory.getOwner()};
	std::string targetOwner{targetTerritory.getOwner()};
	bool sourceHasDiplomacy = game.getDiplomacyMap().count(sourceOwner) &&
		game.getDiplomacyMap().at(sourceOwner) == targetOwner;
	bool targetHasDiplomacy = game.getDiplomacyMap().count(targetOwner) &&
		game.getDiplomacyMap().at(targetOwner) == sourceOwner;
	
	if (sourceOwner != player.getPlayerName()) {
		*orderEffect = "Invalid Order: " + player.getPlayerName() + " doesn't own " + *source + ".";
		return false;
	} else if (game.getMapLoader().getMap().getGameMap().find(&sourceTerritory)->second.count(&targetTerritory) == 0) {
		*orderEffect = "Invalid Order: " + *source + " and " + *target + " aren't neighbors.";
		return false;
	} else if (sourceHasDiplomacy || targetHasDiplomacy) {
		*orderEffect = sourceOwner + " and " + targetOwner + " are currently under diplomacy, so the advance order is invalid.";
		return false;
	}

	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Advance::execute(GameEngine& game, Player& player) {
	Territory& sourceTerritory{game.getMapLoader().getMap().getTerritoryByName(*source)};
	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
	std::string sourceOwner{sourceTerritory.getOwner()};
	std::string targetOwner{targetTerritory.getOwner()};

	if (validate(game, player)) {
		if (*numOfUnits > sourceTerritory.getNumOfUnits()) {
			*numOfUnits = sourceTerritory.getNumOfUnits();
		}
		if (sourceOwner == targetOwner) {
			sourceTerritory.removeUnits(*numOfUnits);
			targetTerritory.addUnits(*numOfUnits);
			*orderEffect = std::to_string(*numOfUnits) + " units were moved from " + *source + " to " + *target + ".";
		} else {
			*orderEffect = sourceOwner + "'s " + *source + " attacks " + targetOwner + "'s " + *target + ".";

			sourceTerritory.removeUnits(*numOfUnits);

			int deadAttackers{static_cast<int>(std::round(targetTerritory.getNumOfUnits() * 0.7))};
			int deadDefenders{static_cast<int>(std::round(*numOfUnits * 0.6))};

			targetTerritory.removeUnits(deadDefenders);
			if (targetTerritory.getNumOfUnits() == 0) {
				game.getPlayerByName(sourceOwner).addTerritory(&targetTerritory);
				if (targetOwner != "Neutral") {
					game.getPlayerByName(targetOwner).removeTerritory(&targetTerritory);
				}
				targetTerritory.setOwner(sourceOwner);
				targetTerritory.addUnits(*numOfUnits - deadAttackers);
				*orderEffect +=  "\n" + sourceOwner + " has conquered " + *target + ".";
				game.getPlayerByName(sourceOwner).getDrawCard() = true;
			} else {
				sourceTerritory.addUnits(*numOfUnits - deadAttackers);
			}
			*orderEffect += "\nBattle results:\nAttacking Territory: " + *source + " - Owner: " + sourceTerritory.getOwner() + " - Units: " + std::to_string(sourceTerritory.getNumOfUnits()) + "."
				+ "\nDefending Territory: " + *target + " - Owner: " + targetTerritory.getOwner() + " - Units: " + std::to_string(targetTerritory.getNumOfUnits()) + ".";
		}
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bomb subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Bomb::Bomb(std::string target)
	: Order{"Destroy half of the army units located on a target territory.\nThis order can only be issued if a player has the bomb card in their hand."},
	target{new std::string{target}} {
	//std::cout << "Called Bomb parameterized contructor" << std::endl;
}

// Copy constructor
Bomb::Bomb(const Bomb& source)
	: Order{source}, target{new std::string{*source.target}} {
	//std::cout << "Called Bomb copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Bomb::~Bomb() {
	delete target;
	//std::cout << "Called Bomb destructor" << std::endl;
}

// Returns a copy of the object
Order* Bomb::clone() const {
	return new Bomb(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Bomb::validate(GameEngine& game, Player& player) const {
	if (game.getMapLoader().getMap().getTerritories().find(*target) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *target + " is not an existing territory.";
		return false;
	}

	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
	std::string targetOwner{targetTerritory.getOwner()};
	
	if (targetTerritory.getOwner() == player.getPlayerName()) {
		*orderEffect = "You cannot bomb your own territory!";
		return false;
	}

	bool attackerHasDiplomacy = game.getDiplomacyMap().count(player.getPlayerName()) &&
		game.getDiplomacyMap().at(player.getPlayerName()) == targetOwner;
	bool targetHasDiplomacy = game.getDiplomacyMap().count(targetOwner) &&
		game.getDiplomacyMap().at(targetOwner) == player.getPlayerName();

	if (attackerHasDiplomacy || targetHasDiplomacy) {
		*orderEffect = player.getPlayerName() + " and " + targetOwner + " are currently under diplomacy, so the bomb order is invalid.";
		return false;
	}

	for (const auto& t : game.getMapLoader().getMap().getGameMap().find(&targetTerritory)->second) {
		if (t->getOwner() == player.getPlayerName()) {
			return true;
		}
	}
	*orderEffect = "None of your territories are neighbors with the target.";
	return false;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Bomb::execute(GameEngine& game, Player& player) {
	if (validate(game, player)) {
		Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
		targetTerritory.setNumOfUnits(targetTerritory.getNumOfUnits() / 2);
		*orderEffect = player.getPlayerName() + " has bombed " + *target
			+ "\nBombing Result:\nTerritory: " + *target + " - Owner : " + targetTerritory.getOwner() 
			+ " - Units : " + std::to_string(targetTerritory.getNumOfUnits()) + ".";
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Blockade subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
Blockade::Blockade(std::string target)
	: Order{"Double the number of army units on a target territory and make it a neutral territory.\nThis order can only be issued if a player has the blockade card in their hand."},
	target{new std::string{target}} {
	//std::cout << "Called Blockade default contructor" << std::endl;
}

// Copy constructor
Blockade::Blockade(const Blockade& source)
	: Order{source}, target{new std::string{*source.target}} {
	//std::cout << "Called Blockade copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Blockade::~Blockade() {
	delete target;
	//std::cout << "Called Blockade destructor" << std::endl;
}

// Returns a copy of the object
Order* Blockade::clone() const {
	return new Blockade(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Blockade::validate(GameEngine& game, Player& player) const {
	if (game.getMapLoader().getMap().getTerritories().find(*target) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *target + " is not an existing territory.";
		return false;
	}

	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
	if (targetTerritory.getOwner() != player.getPlayerName()) {
		*orderEffect = "Order invalid, you don't own the target territory.";
		return false;
	}
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Blockade::execute(GameEngine& game, Player& player) {
	if (validate(game, player)) {
		Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
		targetTerritory.addUnits(targetTerritory.getNumOfUnits());
		targetTerritory.setOwner("Neutral");
		player.removeTerritory(&targetTerritory);
		*orderEffect = "A blockade as been added on " + *target;
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Airlift subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Airlift::Airlift(int numOfUnits, std::string source, std::string target) 
	: Order{"Advance a certain number of army units from one from one territory (source territory) to another territory(target territory).\nThis order can only be issued if a player has the airlift card in their hand."},
	numOfUnits{new int{numOfUnits}}, source{new std::string{source}}, target{new std::string{target}} {
	//std::cout << "Called Airlift parameterized contructor" << std::endl;
}

// Copy constructor
Airlift::Airlift(const Airlift& source)
	: Order{source}, numOfUnits{new int{*source.numOfUnits}}, source{new std::string{*source.source}}, target{new std::string{*source.target}} {
	//std::cout << "Called Airlift copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Airlift::~Airlift() {
	delete numOfUnits;
	delete source;
	delete target;
	//std::cout << "Called Airlift destructor" << std::endl;
}

// Returns a copy of the object
Order* Airlift::clone() const {
	return new Airlift(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Airlift::validate(GameEngine& game, Player& player) const {
	if (game.getMapLoader().getMap().getTerritories().find(*source) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *source + " is not an existing territory.";
		return false;
	}
	if (game.getMapLoader().getMap().getTerritories().find(*target) == game.getMapLoader().getMap().getTerritories().end()) {
		*orderEffect = *target + " is not an existing territory.";
		return false;
	}

	Territory& sourceTerritory{game.getMapLoader().getMap().getTerritoryByName(*source)};
	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};
	if (sourceTerritory.getOwner() != player.getPlayerName() || targetTerritory.getOwner() != player.getPlayerName()) {
		*orderEffect = "Invalid order. You need to own both territories.";
		return false;
	}
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Airlift::execute(GameEngine& game, Player& player) {
	Territory& sourceTerritory{game.getMapLoader().getMap().getTerritoryByName(*source)};
	Territory& targetTerritory{game.getMapLoader().getMap().getTerritoryByName(*target)};

	if (validate(game, player)) {
		if (*numOfUnits > sourceTerritory.getNumOfUnits()) {
			*numOfUnits = sourceTerritory.getNumOfUnits();
		}
		sourceTerritory.removeUnits(*numOfUnits);
		targetTerritory.addUnits(*numOfUnits);
		*orderEffect = std::to_string(*numOfUnits) + " units were moved from " + *source + " to " + *target + ".";
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Negotiate subclass function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Negotiate::Negotiate(std::string target)
	: Order{"Prevent attacks between the current player and another target player until the end of the turn.\nThis order can only be issued if a player has the diplomacy card in their hand."},
	target{new std::string{target}} {
	//std::cout << "Called Negotiate default contructor" << std::endl;
}

// Copy constructor
Negotiate::Negotiate(const Negotiate& source)
	: Order{source}, target{new std::string{*source.target}} {
	//std::cout << "Called Negotiate copy contructor" << std::endl;
}

// Destructor, deallocates memory for all the pointer data members
Negotiate::~Negotiate() {
	delete target;
	//std::cout << "Called Negotiate destructor" << std::endl;
}

// Returns a copy of the object
Order* Negotiate::clone() const {
	return new Negotiate(*this);
}

// Verifies if the order is valid according to the order's meaning and the player's state
bool Negotiate::validate(GameEngine& game, Player& player) const {
	if (game.getPlayerLookup().find(*target) == game.getPlayerLookup().end()) {
		*orderEffect = "Order invalid, player not found.";
		return false;
	}
	if (player.getPlayerName() == *target) {
		*orderEffect = "You can't negotiate with yoursef.";
		return false;
	}
	return true;
}

//  First validates the order, then executes its action if it is valid, according to the order’s meaning and the player’s state
void Negotiate::execute(GameEngine& game, Player& player) {
	if (validate(game, player)) {
		game.getDiplomacyMap().emplace(player.getPlayerName(), *target);
		*orderEffect = player.getPlayerName() + " has negotiated a temporary truce with " + *target + " (both players cannot attack eachother until the next turn).";
	}
	notify(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OrdersList class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
OrdersList::OrdersList() 
	: orders{new std::vector<Order*>}, logObserver{nullptr} {
	//std::cout << "Called OrdersList default constructor" << std::endl;
}

// Copy constructor
OrdersList::OrdersList(const OrdersList& source) 
	: orders{new std::vector<Order*>}, logObserver{nullptr} {
	setLogObserver(source.logObserver);
	for (const Order* order : *source.orders) {
		orders->push_back(order->clone()); // Deep copy
	}
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

		orders = new std::vector<Order*>;
		for (const Order* order : *rhs.orders) {
			orders->push_back(order->clone());
		}
		setLogObserver(rhs.logObserver);
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const OrdersList& obj) {
	os << "Orders:" << std::endl;
	for (const Order* o : obj.getOrders()) {
		os << *o << std::endl;
	}
	return os;
}

// Getters
LogObserver& OrdersList::getLogObserver() {
	return *logObserver;
}

const std::vector<Order*>& OrdersList::getOrders() const {
	return *orders;
}

const LogObserver& OrdersList::getLogObserver() const {
	return *logObserver;
}

// Mutators
// Add order to player's OrdersList
void OrdersList::addOrder(Order* order) {
	orders->emplace_back(order);
	orders->back()->attach(*logObserver);
	notify(*this);
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

void OrdersList::setLogObserver(LogObserver* logObserver) {
	this->logObserver = logObserver;
	attach(*logObserver);
}

// Logs order description of the last order
std::string OrdersList::stringToLog() const {
	return orders->back()->getOrderDescription();
}