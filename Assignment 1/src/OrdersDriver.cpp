#include "Orders.h"

void testOrdersList() {
	OrdersList oList{};
	
	oList.getOrders().emplace_back(new Deploy);
	oList.getOrders().emplace_back(new Advance);
	oList.getOrders().emplace_back(new Bomb);
	oList.getOrders().emplace_back(new Blockade);
	oList.getOrders().emplace_back(new Airlift);
	oList.getOrders().emplace_back(new Negotiate);
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "\n***Orders***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.remove(0);

	std::cout << "\n***Orders after remove***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.move(0, 3);
	std::cout << "\n***Orders after move***" << std::endl << std::endl;
	for (Order* o : oList.getOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "\n***OrdersList***" << std::endl << std::endl;
	std::cout << oList << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}