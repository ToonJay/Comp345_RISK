#include "Orders.h"

void testOrdersList() {
	OrdersList oList{};
	
	oList.GetOrders().emplace_back(new Deploy);
	oList.GetOrders().emplace_back(new Advance);
	oList.GetOrders().emplace_back(new Bomb);
	oList.GetOrders().emplace_back(new Blockade);
	oList.GetOrders().emplace_back(new Airlift);
	oList.GetOrders().emplace_back(new Negotiate);
	std::cout << "---------------------------------------------" << std::endl;
	std::cout << "\n***Orders***" << std::endl << std::endl;
	for (Order* o : oList.GetOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.remove(0);

	std::cout << "\n***Orders after remove***" << std::endl << std::endl;
	for (Order* o : oList.GetOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "---------------------------------------------" << std::endl;
	oList.move(0, 3);
	std::cout << "\n***Orders after move***" << std::endl << std::endl;
	for (Order* o : oList.GetOrders()) {
		o->execute();
		std::cout << *o << std::endl;
	}
	std::cout << "\n***OrdersList***" << std::endl << std::endl;
	std::cout << oList << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
}